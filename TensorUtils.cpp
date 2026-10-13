/**
 * @file TensorUtils.cpp
 * @author Lucas Gasparino (lucas.gasparino3110@gmail.com)
 * @brief Implementation of TensorUtils class methods.
 * @version 0.1
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "TensorUtils.hpp"

template <typename ITYPE, typename RTYPE>
void TensorUtils<ITYPE, RTYPE>::axpy(const ITYPE size, const RTYPE alpha, const RTYPE* __restrict x, RTYPE* __restrict y) {
    for (ITYPE i = 0; i < size; ++i) {
        y[i] += alpha * x[i];
    }
}

template <typename ITYPE, typename RTYPE>
void TensorUtils<ITYPE, RTYPE>::scale(const ITYPE size, const RTYPE alpha, RTYPE* __restrict x) {
    for (ITYPE i = 0; i < size; ++i) {
        x[i] *= alpha;
    }
}

template <typename ITYPE, typename RTYPE>
void TensorUtils<ITYPE, RTYPE>::dot_product(const ITYPE size, const RTYPE* __restrict x, const RTYPE* __restrict y, double* __restrict result) {
    result[0] = static_cast<double>(0);
    double tmp = 0.0;
    for (ITYPE i = 0; i < size; ++i) {
        tmp += static_cast<double>(x[i] * y[i]);
    }
    result[0] = tmp;
}

template <typename ITYPE, typename RTYPE>
void TensorUtils<ITYPE, RTYPE>::multiply_entries(const ITYPE size, const RTYPE* __restrict x, RTYPE* __restrict y) {
    for (ITYPE i = 0; i < size; ++i) {
        y[i] *= x[i];
    }
}

template <typename ITYPE, typename RTYPE>
void TensorUtils<ITYPE, RTYPE>::invert_entries(const ITYPE size, RTYPE* __restrict x) {
    for (ITYPE i = 0; i < size; ++i) {
        x[i] = static_cast<RTYPE>(1) / x[i];
    }
}

template <typename ITYPE, typename RTYPE>
void TensorUtils<ITYPE, RTYPE>::copy_array(const ITYPE size, const RTYPE* __restrict x, RTYPE* __restrict y) {
    for (ITYPE i = 0; i < size; ++i) {
        y[i] = x[i];
    }
}

template <typename ITYPE, typename RTYPE>
void TensorUtils<ITYPE, RTYPE>::vector_matrix_dot(const ITYPE cols, const ITYPE rowIdx, const RTYPE* __restrict x, const RTYPE* __restrict A, double* __restrict result) {
    double tmp = static_cast<double>(0);
    const RTYPE* row_ptr = A + rowIdx * cols;
    for (ITYPE icol = 0; icol < cols; ++icol) {
        tmp += static_cast<double>(x[icol] * row_ptr[icol]);
    }
    result[0] = tmp;
}

template <typename ITYPE, typename RTYPE>
void TensorUtils<ITYPE, RTYPE>::vector_matrix_axpy(const ITYPE cols, const ITYPE rowIdx, const RTYPE alpha, const RTYPE* __restrict A, RTYPE* __restrict y) {
    const RTYPE* row_ptr = A + rowIdx * cols;
    for (ITYPE icol = 0; icol < cols; ++icol) {
        y[icol] += alpha * row_ptr[icol];
    }
}

template class TensorUtils<uint32_t, float>;
template class TensorUtils<uint64_t, float>;
template class TensorUtils<uint32_t, double>;
template class TensorUtils<uint64_t, double>;