#ifndef INTTYPES_HPP
#define INTTYPES_HPP

// Implementation of cstdint header for bare metal environments

// Define fixed-width integer types
typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef short              int16_t;
typedef unsigned short     uint16_t;
typedef int                int32_t;
typedef unsigned int       uint32_t;
typedef long long          int64_t;
typedef unsigned long long uint64_t;

#endif // INTTYPES_HPP