#include "ConjugateGradient.hpp"
#include "GMRES.hpp"
#include "Output.hpp"

int main() {
    uint32_t idx;

    // Generate the matrix
    float c[ARR_SIZE];
    float d[ARR_SIZE];
    float e[ARR_SIZE];
    for (idx = 0; idx < ARR_SIZE; idx++) {
        c[idx] = 1.0f;
        d[idx] = 4.0f;
        e[idx] = 1.0f;
    }

    // Generate x0 and rhs
    float x0[ARR_SIZE];
    float rhs[ARR_SIZE];
    for (idx = 0; idx < ARR_SIZE; idx++) {
        x0[idx] = 0.001f;
        rhs[idx] = static_cast<float>(idx + 1);
    }

    // Create the ConjugateGradient object
    ConjugateGradient<uint32_t, float> solver(x0, rhs);

    // Call the cg() solver with the appropriate matvec operator
    auto MV_Op = [&](const float* __restrict x, float* __restrict Ax) {
        MatVec<uint32_t,float>::cyclic_tridiag(c, d, e, x, Ax, ARR_SIZE);
    };
    solver.cg(MV_Op);

    // Retrieve the solution
    const float* solution = solver.get_solution();

    // Print the solution
    //for (idx = 0; idx < ARR_SIZE; idx++) {
    //    Output::print("solution[%d] = %f\n", idx, solution[idx]);
    //}

    // Verify: do Ax-b and check results
    float Ax[ARR_SIZE];
    float r[ARR_SIZE];
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
    GMRES<uint32_t, float> gmres_solver(x0, rhs);
    gmres_solver.solve_basic(MV_Op);

    return 0;
}