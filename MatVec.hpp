#ifndef MATVEC_H
#define MATVEC_H

#include "IntTypes.hpp"

template<typename ITYPE, typename RTYPE>
class MatVec
{
    public:
        static void cyclic_tridiag(const RTYPE *__restrict c,
                                   const RTYPE *__restrict d,
                                   const RTYPE *__restrict e,
                                   const RTYPE *__restrict x,
                                   RTYPE *__restrict y,
                                   const ITYPE n);
};

#endif