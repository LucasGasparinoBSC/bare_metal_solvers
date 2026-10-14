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
        c[idx] = 1.0f;
        d[idx] = 4.0f;
        e[idx] = 1.0f;
    }

    // Generate x0 and rhs
    double x0[ARR_SIZE];
    double rhs[ARR_SIZE];
    for (idx = 0; idx < ARR_SIZE; idx++) {
        x0[idx] = 0.001f;
        rhs[idx] = static_cast<double>(idx + 1);
    }

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