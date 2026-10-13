#ifndef MATHFUNC_HPP
#define MATHFUNC_HPP

template<typename VTYPE>
class MathFunc
{
    public:
        static VTYPE sqrt(const VTYPE val) {
            if (val < 0) {
                return -1;
            }
            if constexpr (sizeof(VTYPE) == sizeof(float)) {
                return __builtin_sqrtf(val);
            } else {
                return __builtin_sqrt(val);
            }
        }

        static VTYPE abs(const VTYPE val) {
            if constexpr (sizeof(VTYPE) == sizeof(float)) {
                return __builtin_fabsf(val);
            } else {
                return __builtin_fabs(val);
            }
        }

        static VTYPE hypot(const VTYPE x, const VTYPE y) {
            VTYPE zero = static_cast<VTYPE>(0);
            VTYPE one = static_cast<VTYPE>(1);
            VTYPE abs_x = abs(x);
            VTYPE abs_y = abs(y);

            if (abs_x == zero) return abs_y;
            if (abs_y == zero) return abs_x;

            // Scaling to prevent overflow/underflow
            if (abs_x > abs_y) {
                VTYPE tmp = abs_y / abs_x;
                tmp = one + (tmp * tmp);
                tmp = sqrt(tmp);
                return abs_x * tmp;
            } else if (abs_y > abs_x) {
                VTYPE tmp = abs_x / abs_y;
                tmp = one + (tmp * tmp);
                tmp = sqrt(tmp);
                return abs_y * tmp;
            } else {
                // abs_x == abs_y
                return abs_x; // or abs_y, they are equal
            }
        }
};

#endif // MATHFUNC_HPP