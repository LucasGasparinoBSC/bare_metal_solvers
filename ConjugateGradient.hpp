#ifndef CONJUGATEGRADIENT_HPP
#define CONJUGATEGRADIENT_HPP

#include "IntTypes.hpp"
#include "TensorUtils.hpp"
#include "MathFunc.hpp"
#include "MatVec.hpp"
#include "Output.hpp"

#ifndef ARR_SIZE
    #define ARR_SIZE 10
#endif

#ifndef MITERS
    #define MITERS 12
#endif

template<typename ITYPE, typename RTYPE>
class ConjugateGradient
{
    private:
        const ITYPE maxIters = MITERS;
        const double tol = 1e-6;
        const double zeroFP64 = static_cast<double>(0);
        const RTYPE zero = static_cast<RTYPE>(0);
        const RTYPE one = static_cast<RTYPE>(1);
        const RTYPE negOne = static_cast<RTYPE>(-1);
        const RTYPE *x0;
        const RTYPE *b;
        ITYPE iter;
        RTYPE rk[ARR_SIZE];
        RTYPE pk[ARR_SIZE];
        RTYPE zk[ARR_SIZE];
        RTYPE Ax[ARR_SIZE];
        RTYPE x_sol[ARR_SIZE];
        RTYPE alpha_rt[1];
        RTYPE beta_rt[1];
        double alpha[1];
        double beta[1];
        double resK[1];
        double res0[1];

    public:
        ConjugateGradient();
        ConjugateGradient(const RTYPE* __restrict xinit, const RTYPE* __restrict rhs);
        ~ConjugateGradient();

        void setup(const RTYPE* __restrict xinit, const RTYPE* __restrict rhs);

        // Getter for x_sol
        const RTYPE* get_solution() const {
            return x_sol;
        }

        template<typename MatVecOp>
        void cg(MatVecOp&& MV_Op)
        {

            Output::print("Starting Conjugate Gradient solver...\n");

            // Set x_sol to x0
            TensorUtils<ITYPE, RTYPE>::copy_array(ARR_SIZE, x0, x_sol);

            // Compute the matvec
            MV_Op(x_sol, Ax);

            // Compute the initial residual r0 = b - A * x0
            TensorUtils<ITYPE, RTYPE>::copy_array(ARR_SIZE, b, rk);
            TensorUtils<ITYPE, RTYPE>::axpy(ARR_SIZE, negOne, Ax, rk);

            // Set p0 = r0
            TensorUtils<ITYPE, RTYPE>::copy_array(ARR_SIZE, rk, pk);

            // Compute resK and res0
            TensorUtils<ITYPE, RTYPE>::dot_product(ARR_SIZE, rk, rk, resK);
            res0[0] = MathFunc<double>::sqrt(resK[0]);
            Output::print("Initial residual norm = %f\n", res0[0]);

            // Iterate
            for (iter = 0; iter < maxIters; ++iter) {
                // Compute the matvec
                MV_Op(pk, Ax);

                // Compute alpha = (rk' * rk) / (pk' * A * pk)
                TensorUtils<ITYPE, RTYPE>::dot_product(ARR_SIZE, pk, Ax, alpha);
                alpha[0] = resK[0] / alpha[0];
                alpha_rt[0] = static_cast<RTYPE>(alpha[0]);

                // Update x_sol = x_sol + alpha * pk
                TensorUtils<ITYPE, RTYPE>::axpy(ARR_SIZE, alpha_rt[0], pk, x_sol);

                // Update rk = rk - alpha * A * pk
                TensorUtils<ITYPE, RTYPE>::axpy(ARR_SIZE, -alpha_rt[0], Ax, rk);

                // Compute new resK
                TensorUtils<ITYPE, RTYPE>::dot_product(ARR_SIZE, rk, rk, beta);

                // Check for convergence
                if (MathFunc<double>::sqrt(beta[0]) < tol*res0[0]) {
                    Output::print("Converged after %d iterations with residual norm = %f\n", iter + 1, MathFunc<double>::sqrt(beta[0]));
                    break;
                }
                Output::print("Iteration %d: residual norm = %f\n", iter + 1, MathFunc<double>::sqrt(beta[0]));

                // Compute beta = (rk' * rk) / (previous rk' * previous rk)
                beta[0] = beta[0] / resK[0];
                beta_rt[0] = static_cast<RTYPE>(beta[0]);

                // Update pk = rk + beta * pk
                TensorUtils<ITYPE, RTYPE>::copy_array(ARR_SIZE, pk, zk);
                TensorUtils<ITYPE, RTYPE>::scale(ARR_SIZE, beta_rt[0], zk);
                TensorUtils<ITYPE, RTYPE>::axpy(ARR_SIZE, one, rk, zk);
                TensorUtils<ITYPE, RTYPE>::copy_array(ARR_SIZE, zk, pk);
                resK[0] = beta[0] * resK[0];
            }
        }

        void pcg();
        void fpcg();
};

#endif // CONJUGATEGRADIENT_HPP