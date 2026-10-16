#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "IntTypes.hpp"

class Output
{
private:
#ifndef __riscv
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

    static inline void raw_print_double(double val, int precision = 6)
    {
        if (val < 0)
        {
            raw_putchar('-');
            val = -val;
        }

        // 1. Properly round the entire number to the desired precision first
        // to prevent cascading truncation bugs (e.g., 0.9999999 -> 1.000000)
        double scale = 1.0;
        for (int i = 0; i < precision; i++)
        {
            scale *= 10.0;
        }

        // Add 0.5 to round up properly before ripping out the integer segment
        uint64_t total_rounded = (uint64_t)(val * scale + 0.5);
        uint64_t int_part = total_rounded / (uint64_t)scale;
        uint64_t frac_part = total_rounded % (uint64_t)scale;

        // 2. Print the integer part
        raw_print_int(int_part);
        raw_putchar('.');

        // 3. Manually pad leading zeros for the fractional part
        uint64_t padding_check = (uint64_t)scale / 10;
        while (padding_check > 0 && frac_part < padding_check)
        {
            raw_putchar('0');
            padding_check /= 10;
        }

        // 4. Print the remaining digits (if frac_part is 0, print nothing more since we padded zeros)
        if (frac_part > 0 || precision == 0)
        {
            raw_print_int(frac_part);
        }
        else
        {
            // If frac_part was exactly 0, we already printed all '0's via the padding loop except the last one
            raw_putchar('0');
        }
    }

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
                format++;
                char spec = *format++;

                print_typed(spec, value);

                print_impl(format, args...);
                return;
            }
            raw_putchar(*format++);
        }
    }
#endif // __riscv

public:
    template <typename... Args>
    static inline void print(const char *format, Args... args)
    {
#ifndef __riscv
        print_impl(format, args...);
#else
        // No-op during RISC-V compilation to prevent unsupported x86 syscall errors
        (void)format;
        // Unpack arguments to avoid unused parameter warnings if necessary
        int dummy[] = {0, ((void)(args), 0)...};
        (void)dummy;
#endif
    }
};

#endif // OUTPUT_HPP