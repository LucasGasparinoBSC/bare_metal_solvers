#include "ConjugateGradient.hpp"

template<typename ITYPE, typename RTYPE>
ConjugateGradient<ITYPE, RTYPE>::ConjugateGradient()
{
    x0 = nullptr;
    b = nullptr;
}

template<typename ITYPE, typename RTYPE>
ConjugateGradient<ITYPE, RTYPE>::ConjugateGradient(const RTYPE* __restrict xinit, const RTYPE* __restrict rhs)
{
    setup(xinit, rhs);
}

template<typename ITYPE, typename RTYPE>
ConjugateGradient<ITYPE, RTYPE>::~ConjugateGradient()
{
    // No dynamic memory allocation, so nothing to clean up
}

template<typename ITYPE, typename RTYPE>
void ConjugateGradient<ITYPE, RTYPE>::setup(const RTYPE* __restrict xinit, const RTYPE* __restrict rhs)
{
    x0 = xinit;
    b = rhs;
}

template class ConjugateGradient<uint32_t, float>;
template class ConjugateGradient<uint64_t, float>;
template class ConjugateGradient<uint32_t, double>;
template class ConjugateGradient<uint64_t, double>;