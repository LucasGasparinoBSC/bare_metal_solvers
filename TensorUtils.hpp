/**
 * @file TensorUtils.hpp
 * @author Lucas Gasparino (lucas.gasparino3110@gmail.com)
 * @brief Set of utility functions for performing typical tensor operations.
 * @details This class provides static methods for common tensor operations
 *          such as AXPY, scaling, dot product, element-wise multiplication,
 *          and more. The templating allows for flexibility in the choice of
 *          integer and real number types. Since this is intented for CPU
 *          execution, no half precision support is provided.
 *          Other operations can be added as required.
 * @version 0.1
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef TENSORUTILS_HPP
#define TENSORUTILS_HPP

#pragma once

#include "IntTypes.hpp"

// Declaration of TensorUtils class
// Template types:
// ITYPE - Integer type for sizes and indices (e.g., int32_t, int64_t)
// RTYPE - Real number type for tensor values (float, double)
template <typename ITYPE, typename RTYPE>
class TensorUtils
{
    public:
        // axpy: Performs the operation y = alpha * x + y
        static void axpy(const ITYPE size, const RTYPE alpha, const RTYPE* __restrict x, RTYPE* __restrict y);

        // scale: Scales the vector x by alpha (x = alpha * x)
        static void scale(const ITYPE size, const RTYPE alpha, RTYPE* __restrict x);

        // dot_product: Computes the dot product of vectors x and y
        static void dot_product(const ITYPE size, const RTYPE* __restrict x, const RTYPE* __restrict y, double* __restrict result);

        // multiply_entries: Performs element-wise multiplication of vectors x and y (y = x * y)
        static void multiply_entries(const ITYPE size, const RTYPE* __restrict x, RTYPE* __restrict y);

        // invert_entries: Inverts each entry of vector x (x = 1 / x)
        static void invert_entries(const ITYPE size, RTYPE* __restrict x);

        // copy_array: Copies the contents of array x to array y
        static void copy_array(const ITYPE size, const RTYPE* __restrict x, RTYPE* __restrict y);

        // Vector Matrix dot product: Computes the dot product of a vector and a matrix (result = x[j] * A[j][:])
        // NOTE: Assumes A is stored in column-major order!
        static void vector_matrix_dot(const ITYPE cols, const ITYPE rowIdx, const RTYPE* __restrict x, const RTYPE* __restrict A, double* __restrict result);

        // Vector Matrix AXPY: Computes the operation y[j] = y[j] + alpha * A[j][:]
        // NOTE: Assumes A is stored in column-major order!
        static void vector_matrix_axpy(const ITYPE cols, const ITYPE rowIdx, const RTYPE alpha, const RTYPE* __restrict A, RTYPE* __restrict y);
};

#endif // TENSORUTILS_HPP