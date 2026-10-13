#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "IntTypes.hpp"

class Output
{
    private:
        static inline void raw_putchar(char c)
        {
            asm volatile(
                "syscall"
                :
                : "a"(1), "D"(1), "S"(&c), "d"(1)
                : "rcx", "r11", "memory");
        }

        static inline void raw_print(const char *str)
        {
            while (*str)
            {
                raw_putchar(*str++);
            }
        }

        static inline void raw_print_int(int64_t val)
        {
            char buf[32];
            int i = 31;
            bool negative = false;

            if (val == 0)
            {
                raw_putchar('0');
                return;
            }
            if (val < 0)
            {
                negative = true;
                val = -val;
            }

            buf[i--] = '\0';
            while (val > 0)
            {
                buf[i--] = '0' + (val % 10);
                val /= 10;
            }
            if (negative)
            {
                buf[i--] = '-';
            }
            raw_print(&buf[i + 1]);
        }

        static inline void raw_print_double(double val, int precision = 4)
        {
            if (val < 0)
            {
                raw_putchar('-');
                val = -val;
            }
            int64_t int_part = (int64_t)val;
            raw_print_int(int_part);
            raw_putchar('.');

            double frac_part = val - (double)int_part;
            for (int i = 0; i < precision; i++)
            {
                frac_part *= 10.0;
            }
            raw_print_int((int64_t)(frac_part + 0.5));
        }

        // Overloaded type dispatchers prevent invalid compile-time casts in templates
        static inline void print_typed(char spec, int64_t val)
        {
            if (spec == 'd' || spec == 'i')
                raw_print_int(val);
            else if (spec == 'c')
                raw_putchar((char)val);
        }
        static inline void print_typed(char spec, uint32_t val)
        {
            if (spec == 'd' || spec == 'i')
                raw_print_int((int64_t)val);
            else if (spec == 'c')
                raw_putchar((char)val);
        }
        static inline void print_typed(char spec, int val)
        {
            if (spec == 'd' || spec == 'i')
                raw_print_int((int64_t)val);
            else if (spec == 'c')
                raw_putchar((char)val);
        }
        static inline void print_typed(char spec, double val)
        {
            if (spec == 'f')
                raw_print_double(val);
        }
        static inline void print_typed(char spec, float val)
        {
            if (spec == 'f')
                raw_print_double((double)val);
        }
        static inline void print_typed(char spec, const char *val)
        {
            if (spec == 's')
                raw_print(val);
        }

        // Recursive variadic implementation (Private)
        static inline void print_impl(const char *format)
        {
            while (*format)
            {
                raw_putchar(*format++);
            }
        }

        template <typename T, typename... Args>
        static inline void print_impl(const char *format, T value, Args... args)
        {
            while (*format)
            {
                if (*format == '%' && *(format + 1) != '%')
                {
                    format++; // skip '%'
                    char spec = *format++;

                    print_typed(spec, value);

                    print_impl(format, args...);
                    return;
                }
                raw_putchar(*format++);
            }
        }

    public:
        template <typename... Args>
        static inline void print(const char *format, Args... args)
        {
            print_impl(format, args...);
        }
};

#endif // OUTPUT_HPP