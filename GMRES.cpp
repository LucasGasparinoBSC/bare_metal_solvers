#include "GMRES.hpp"

template<typename ITYPE, typename RTYPE>
GMRES<ITYPE, RTYPE>::GMRES()
{
    x0 = nullptr;
    b = nullptr;
}

template<typename ITYPE, typename RTYPE>
GMRES<ITYPE, RTYPE>::GMRES(const RTYPE* __restrict xinit, const RTYPE* __restrict rhs)
{
    setup(xinit, rhs);
}

template<typename ITYPE, typename RTYPE>
GMRES<ITYPE, RTYPE>::~GMRES()
{
    // No dynamic memory allocation, so nothing to clean up
}

template<typename ITYPE, typename RTYPE>
void GMRES<ITYPE, RTYPE>::setup(const RTYPE* __restrict xinit, const RTYPE* __restrict rhs)
{
    Output::print("Setting up GMRES solver...\n");
    x0 = xinit;
    b = rhs;

    // Initialize Q_k to 0
    for (ITYPE i = 0; i < (MITERS + 1) * ARR_SIZE; ++i)
    {
        Q_k[i] = zero;
    }

    // Initialize H_k to 0
    for (ITYPE i = 0; i < (MITERS + 1) * MITERS; ++i)
    {
        H_k[i] = zeroFP64;
    }

    // Initialize Givens rotation coefficients to 0
    for (ITYPE i = 0; i < MITERS; ++i)
    {
        cs[i] = zeroFP64;
        sn[i] = zeroFP64;
    }
}

template class GMRES<uint32_t, float>;
template class GMRES<uint64_t, float>;
template class GMRES<uint32_t, double>;
template class GMRES<uint64_t, double>;