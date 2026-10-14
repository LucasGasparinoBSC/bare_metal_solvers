#ifndef GMRES_HPP
#define GMRES_HPP

#include "IntTypes.hpp"
#include "TensorUtils.hpp"
#include "MathFunc.hpp"
#include "MatVec.hpp"
#include "Output.hpp"

#ifndef ARR_SIZE
    #define ARR_SIZE 10
#endif

#ifndef MITERS
    #define MITERS 10
#endif

template<typename ITYPE, typename RTYPE>
class GMRES
{
    private:
        const ITYPE maxIters = MITERS;
        const double tol = 1e-6;
        const double zeroFP64 = static_cast<double>(0);
        const double oneFP64 = static_cast<double>(1);
        const RTYPE zero = static_cast<RTYPE>(0);
        const RTYPE one = static_cast<RTYPE>(1);
        const RTYPE negOne = static_cast<RTYPE>(-1);
        const RTYPE *x0;                    // Initial guess
        const RTYPE *b;                     // RHS
        bool arnoldiConverged;              // Flag for Arnoldi convergence
        ITYPE kIter;                        // Iteration counter
        double norm[1];                     // L2 norm ptr
        double b_norm[1];                   // L2 norm ptr for RHS
        double hk[1];                       // H_k entries ptr
        double cs[MITERS];                  // Cosine values for Givens rotations
        double sn[MITERS];                  // Sine values for Givens rotations
        double gk[MITERS+1];                // Norm tracker
        RTYPE wk[ARR_SIZE];                 // Auxiliary vector for Arnoldi process
        RTYPE qk[ARR_SIZE];                 // Current Krylov vector
        RTYPE x_sol[ARR_SIZE];              // Solution vector
        RTYPE Ax[ARR_SIZE];                 // Matrix-vector product
        RTYPE Q_k[(MITERS + 1) * ARR_SIZE]; // m+1 x n (transposed for column-major storage)
        double H_k[MITERS * (MITERS + 1)];  // m x m+1 (transposed for column-major storage)

        // Compute rotation coeafficients for Givens rotation
        void compute_givens_rotation(const double v1, const double v2, double &cs_k, double &sn_k)
        {
            if (v2 == 0.0) {
                cs_k = 1.0;
                sn_k = 0.0;
            } else if (v1 == 0.0) {
                cs_k = 0.0;
                sn_k = -1.0;
            } else {
                double r = MathFunc<double>::hypot(v1, v2);
                cs_k = v1 / r;
                sn_k = -v2 / r;
            }
        }

        // Apply Givens rotation to the Hessenberg matrix
        void apply_givens_rotation(const ITYPE k)
        {
            double* hrow = H_k + k * (MITERS + 1);
            for (ITYPE i = 0; i < k; ++i) {
                double temp = cs[i] * hrow[i] - sn[i] * hrow[i + 1];
                hrow[i + 1] = sn[i] * hrow[i] + cs[i] * hrow[i + 1];
                hrow[i] = temp;
            }

            // Compute the new Givens rotation
            compute_givens_rotation(hrow[k], hrow[k + 1], cs[k], sn[k]);

            // Eliminate the sub-diagonal entry
            double temp = cs[k] * hrow[k] - sn[k] * hrow[k + 1];
            hrow[k + 1] = 0.0;
            hrow[k] = temp;
        }

        // Arnoldi process to produce Q_k and H_k entries
        template <typename MatVecOp>
        bool arnoldi(MatVecOp&& MV_Op, const ITYPE k)
        {

            // Compute wk = A * qk
            MV_Op(qk, wk);

            // Compute H_k entries
            for (ITYPE j = 0; j <= k; ++j) {
                // h_jk = dot(wk, Q_k[:, j])
                TensorUtils<ITYPE, RTYPE>::vector_matrix_dot(ARR_SIZE, j, wk, Q_k, hk);
                H_k[k * (MITERS + 1) + j] = hk[0];
                Output::print("--| H_k[%d][%d] = %f\n", k, j, hk[0]);

                // wk = wk - h_jk * Q_k[:, j]
                TensorUtils<ITYPE, RTYPE>::vector_matrix_axpy(ARR_SIZE, j, static_cast<RTYPE>(-hk[0]), Q_k, wk);
            }

            // h_(k+1)k = ||wk||
            TensorUtils<ITYPE, RTYPE>::dot_product(ARR_SIZE, wk, wk, hk);
            hk[0] = MathFunc<double>::sqrt(hk[0]);
            Output::print("--| H_k[%d][%d] = %f\n", k, k + 1, hk[0]);

            // Check for breakdown
            if (hk[0] < 1e-14) {
                // TODO: handle breakdown (e.g., restart or terminate)
                // Happy breakdown: the Krylov subspace has been fully explored
                return true;
            }
            H_k[k * (MITERS + 1) + (k + 1)] = hk[0];

            // q_(k+1) = wk / h_(k+1)k
            hk[0] = oneFP64 / hk[0];
            TensorUtils<ITYPE, RTYPE>::scale(ARR_SIZE, static_cast<RTYPE>(hk[0]), wk);
            for (ITYPE i = 0; i < ARR_SIZE; ++i) {
                Q_k[(k+1) * ARR_SIZE + i] = wk[i];
                qk[i] = wk[i];
            }

            return false;
        }

    public:
        GMRES();
        GMRES(const RTYPE* __restrict xinit, const RTYPE* __restrict rhs);
        ~GMRES();

        void setup(const RTYPE* __restrict xinit, const RTYPE* __restrict rhs);

        // Getter for x_sol
        const RTYPE* get_solution() const {
            return x_sol;
        }

        template<typename MatVecOp>
        void solve_basic(MatVecOp&& MV_Op)
        {
            Output::print("Starting GMRES solver...\n");
            // Set x_sol to x0
            TensorUtils<ITYPE, RTYPE>::copy_array(ARR_SIZE, x0, x_sol);

            // Compute the matvec
            MV_Op(x_sol, Ax);

            // Compute b_norm = ||b||
            TensorUtils<ITYPE, RTYPE>::dot_product(ARR_SIZE, b, b, b_norm);
            b_norm[0] = MathFunc<double>::sqrt(b_norm[0]);

            // Initial residual r0 = b - A * x0
            TensorUtils<ITYPE, RTYPE>::copy_array(ARR_SIZE, b, qk);
            TensorUtils<ITYPE, RTYPE>::axpy(ARR_SIZE, negOne, Ax, qk);

            // q0 = r0 / ||r0||
            TensorUtils<ITYPE, RTYPE>::dot_product(ARR_SIZE, qk, qk, norm);
            norm[0] = MathFunc<double>::sqrt(norm[0]);
            gk[0] = norm[0];
            Output::print("Initial residual norm = %f\n", norm[0]);

            // Check if x0 already satisfies the tolerance
            if (norm[0] < tol) {
                Output::print("Initial guess is already within tolerance.\n");
                return; // x0 is already a good enough solution
            }

            // Compute first Krylov vector q0 = r0 / ||r0||
            norm[0] = oneFP64 / norm[0];
            TensorUtils<ITYPE, RTYPE>::scale(ARR_SIZE, static_cast<RTYPE>(norm[0]), qk);

            // Store q0 in Q_k
            for (ITYPE i = 0; i < ARR_SIZE; ++i) {
                Q_k[0 * ARR_SIZE + i] = qk[i];
            }

            // Iterate for k = 0 to maxIters
            kIter = 0;
            while (kIter < maxIters) {

                // Call Arnoldi process to compute Q_k and H_k
                arnoldiConverged = arnoldi(MV_Op, kIter);

                // Check if arnoldi converged (happy breakdown)
                if (arnoldiConverged) {
                    // Break out of the loop if Arnoldi converged
                    Output::print("Arnoldi process converged at iteration %d\n", kIter);
                    break;
                }

                // Apply Givens rotations to H_k
                apply_givens_rotation(kIter);

                // Update gk
                gk[kIter + 1] = sn[kIter] * gk[kIter];
                gk[kIter] = cs[kIter] * gk[kIter];

                // Check convergence
                double relTol = MathFunc<double>::abs(gk[kIter + 1]) / b_norm[0];
                Output::print("GMRES iteration %d: relative tolerance = %f\n", kIter, relTol);
                if (relTol < tol) {
                    Output::print("GMRES converged at iteration %d with relative tolerance %f\n", kIter, relTol);
                    break; // Converged
                }
                kIter++;
            }

            for (ITYPE j = 0; j < kIter + 1; ++j) {
                Output::print("gk[%d] = %f\n", j, gk[j]);
            }
        }

};

#endif // GMRES_HPP