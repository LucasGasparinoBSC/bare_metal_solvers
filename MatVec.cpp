#include "MatVec.hpp"

template<typename ITYPE, typename RTYPE>
void MatVec<ITYPE, RTYPE>::cyclic_tridiag(const RTYPE *__restrict c,
                                          const RTYPE *__restrict d,
                                          const RTYPE *__restrict e,
                                          const RTYPE *__restrict x,
                                          RTYPE *__restrict y,
                                          const ITYPE n) {
                                            ITYPE idx = 0;
                                            // Inner nodes
                                            for (idx = 1; idx < n - 1; ++idx) {
                                                y[idx] = c[idx-1]*x[idx-1] + d[idx]*x[idx] + e[idx+1]*x[idx+1];
                                            }

                                            // End points
                                            y[0] = d[0]*x[0] + e[1]*x[1] + c[n-1]*x[n-1];
                                            y[n-1] = c[n-2]*x[n-2] + d[n-1]*x[n-1] + e[0]*x[0];
                                        }

template<typename ITYPE, typename RTYPE>
void MatVec<ITYPE, RTYPE>::diag_precond(const RTYPE *__restrict d,
                                        const RTYPE *__restrict x,
                                        RTYPE *__restrict y,
                                        const ITYPE n) {
                                            for (ITYPE idx = 0; idx < n; ++idx) {
                                                y[idx] = x[idx] / d[idx];
                                            }
                                        }

template class MatVec<uint32_t, float>;
template class MatVec<uint64_t, float>;
template class MatVec<uint32_t, double>;
template class MatVec<uint64_t, double>;