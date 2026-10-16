#include "ConjugateGradient.hpp"
#include "GMRES.hpp"
#include "Output.hpp"

int main()
{
    uint32_t idx;

    Output::print("Starting main program...\n");
    Output::print("ARR_SIZE = %d, MITERS = %d\n", ARR_SIZE, MITERS);

    // Generate the matrix
    float c[ARR_SIZE];
    float d[ARR_SIZE];
    float e[ARR_SIZE];
    float h = 1.0 / (static_cast<float>(ARR_SIZE) + 1.0);
    float hh = h * h;
    float khh = 1.0 / hh;
    float fact = 1.1;
    for (idx = 0; idx < ARR_SIZE; idx++)
    {
        c[idx] = khh * -1.0;
        d[idx] = (khh * 2.0) + fact;
        e[idx] = khh * -1.0;
    }

    // Generate x0 and rhs
    float x0[ARR_SIZE];
    float x_ex[ARR_SIZE];
    float rhs[ARR_SIZE];
    for (idx = 0; idx < ARR_SIZE; idx++)
    {
        x0[idx] = 0.001f;
        double val = static_cast<double>(idx) - static_cast<double>(ARR_SIZE) / 2.0;
        double tmp = 1.0 - (4.0 / (static_cast<double>(ARR_SIZE) * static_cast<double>(ARR_SIZE))) * (val * val);
        x_ex[idx] = static_cast<float>(tmp);
    }
    // Use matvec to compute rhs = A * x_ex
    MatVec<uint32_t, float>::cyclic_tridiag(c, d, e, x_ex, rhs, ARR_SIZE);

    // Create the ConjugateGradient object
    ConjugateGradient<uint32_t, float> solver(x0, rhs);

    // Call the cg() solver with the appropriate matvec operator
    auto MV_Op = [&](const float *__restrict x, float *__restrict Ax)
    {
        MatVec<uint32_t, float>::cyclic_tridiag(c, d, e, x, Ax, ARR_SIZE);
    };
    auto Prec_Op = [&](const float *__restrict r, float *__restrict z)
    {
        MatVec<uint32_t, float>::diag_precond(d, r, z, ARR_SIZE);
    };

    // solver.cg(MV_Op);
    solver.pcg(MV_Op, Prec_Op);

    // Retrieve the solution
    const float *solution = solver.get_solution();

    // Print the solution
    // for (idx = 0; idx < ARR_SIZE; idx++) {
    //    Output::print("solution[%d] = %f\n", idx, solution[idx]);
    //}

    // Verify: do Ax-b and check results
    float Ax[ARR_SIZE];
    float r[ARR_SIZE];
    MV_Op(solution, Ax);
    for (idx = 0; idx < ARR_SIZE; idx++)
    {
        r[idx] = Ax[idx] - rhs[idx];
        if (r[idx] < 0.0f)
            r[idx] = -r[idx];

        // HW trap
        if (r[idx] > 1e-4f)
        {
            while (1)
            {
                asm volatile("nop");
            }
        }
    }

    // Now for running the GMRES
    GMRES<uint32_t, float> gmres_solver(x0, rhs);
    //gmres_solver.solve_basic(MV_Op);
    gmres_solver.right_prec(MV_Op, Prec_Op);
    Output::print("GMRES solver completed.\n");
    const float *gmres_solution = gmres_solver.get_solution();

    // Print the GMRES solution
    for (idx = 0; idx < ARR_SIZE; idx++)
    {
        Output::print("gmres_solution[%d] = %f\n", idx, gmres_solution[idx]);
    }

    // Redo the same verification for GMRES
    MV_Op(gmres_solution, Ax);
    for (idx = 0; idx < ARR_SIZE; idx++)
    {
        r[idx] = Ax[idx] - rhs[idx];
        if (r[idx] < 0.0f)
            r[idx] = -r[idx];
        // HW trap
        if (r[idx] > 1e-4f)
        {
            while (1)
            {
                asm volatile("nop");
            }
        }
    }

    // Compare the solutions from CG and GMRES
    for (idx = 0; idx < ARR_SIZE; idx++)
    {
        float diff = MathFunc<float>::abs(solution[idx] - gmres_solution[idx]);
        if (diff > 1e-4f)
        {
            Output::print("Difference between CG and GMRES solutions at index %d is too large: %f\n", idx, diff);
            while (1)
            {
                asm volatile("nop");
            }
        }
        else
        {
            Output::print("Difference between CG and GMRES solutions at index %d is acceptable: %f\n", idx, diff);
        }
    }

    return 0;
}