#include "ConjugateGradient.hpp"
#include "GMRES.hpp"
#include "Output.hpp"

int main() {
    uint32_t idx;

    // Generate the matrix
    double c[ARR_SIZE];
    double d[ARR_SIZE];
    double e[ARR_SIZE];
    for (idx = 0; idx < ARR_SIZE; idx++) {
        c[idx] = 1.0;
        d[idx] = 4.0;
        e[idx] = 1.0;
    }

    // Generate x0 and rhs
    double x0[ARR_SIZE];
    double x_ex[ARR_SIZE];
    double rhs[ARR_SIZE];
    for (idx = 0; idx < ARR_SIZE; idx++)
    {
        x0[idx] = 0.001f;
        double val = static_cast<double>(idx) - static_cast<double>(ARR_SIZE) / 2.0;
        double tmp = 1.0 - (4.0 / (static_cast<double>(ARR_SIZE) * static_cast<double>(ARR_SIZE))) * (val * val);
        x_ex[idx] = static_cast<double>(tmp);
    }
    // Use matvec to compute rhs = A * x_ex
    MatVec<uint32_t, double>::cyclic_tridiag(c, d, e, x_ex, rhs, ARR_SIZE);

    // Create the ConjugateGradient object
    ConjugateGradient<uint32_t, double> solver(x0, rhs);

    // Call the cg() solver with the appropriate matvec operator
    auto MV_Op = [&](const double* __restrict x, double* __restrict Ax) {
        MatVec<uint32_t,double>::cyclic_tridiag(c, d, e, x, Ax, ARR_SIZE);
    };
    solver.cg(MV_Op);

    // Retrieve the solution
    const double* solution = solver.get_solution();

    // Print the solution
    //for (idx = 0; idx < ARR_SIZE; idx++) {
    //    Output::print("solution[%d] = %f\n", idx, solution[idx]);
    //}

    // Verify: do Ax-b and check results
    double Ax[ARR_SIZE];
    double r[ARR_SIZE];
    MV_Op(solution, Ax);
    for (idx = 0; idx < ARR_SIZE; idx++) {
        r[idx] = Ax[idx] - rhs[idx];
        if (r[idx] < 0.0f) r[idx] = -r[idx];

        // HW trap
        if (r[idx] > 1e-4f) {
            while(1) {
                asm volatile("nop");
            }
        }
    }

    // Now for running the GMRES
    GMRES<uint32_t, double> gmres_solver(x0, rhs);
    gmres_solver.solve_basic(MV_Op);

    return 0;
}