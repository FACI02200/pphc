/*
 * PPH Types - Portable integer type definitions
 * Copyright (c) 2025 OpenPajak Contributors
 */

#ifndef PPH_TYPES_H
#define PPH_TYPES_H

/* Compiler and platform detection */
#if defined(__WATCOMC__)
    /* OpenWatcom: Use native __int64 (all versions since 11.0) */
    typedef __int64 pph_int64_t;
    typedef unsigned __int64 pph_uint64_t;
    typedef __int32 pph_int32_t;
    typedef unsigned __int32 pph_uint32_t;
    typedef __int16 pph_int16_t;
    typedef unsigned __int16 pph_uint16_t;
    typedef __int8 pph_int8_t;
    typedef unsigned __int8 pph_uint8_t;

    /* Literal suffixes */
    #define PPH_INT64_C(x)  (x##i64)
    #define PPH_UINT64_C(x) (x##ui64)

    /* OpenWatcom doesn't have inline keyword in C mode */
    #if !defined(__cplusplus)
        #define PPH_INLINE __inline
    #else
        #define PPH_INLINE inline
    #endif

#elif defined(_MSC_VER)
    /* Microsoft Visual C++: Use native __int64 (VC++ 4.2+) */
    typedef __int64 pph_int64_t;
    typedef unsigned __int64 pph_uint64_t;
    typedef __int32 pph_int32_t;
    typedef unsigned __int32 pph_uint32_t;
    typedef __int16 pph_int16_t;
    typedef unsigned __int16 pph_uint16_t;
    typedef __int8 pph_int8_t;
    typedef unsigned __int8 pph_uint8_t;

    /* Literal suffixes (same as OpenWatcom) */
    #define PPH_INT64_C(x)  (x##i64)
    #define PPH_UINT64_C(x) (x##ui64)

    /* MSVC inline keyword */
    #if _MSC_VER >= 1200  /* VC++ 6.0+ */
        #define PPH_INLINE __inline
    #else
        #define PPH_INLINE static
    #endif

#elif defined(__GNUC__) || defined(__clang__)
    /* GCC and Clang: Use C99 standard types */
    #include <stdint.h>

    typedef int64_t pph_int64_t;
    typedef uint64_t pph_uint64_t;
    typedef int32_t pph_int32_t;
    typedef uint32_t pph_uint32_t;
    typedef int16_t pph_int16_t;
    typedef uint16_t pph_uint16_t;
    typedef int8_t pph_int8_t;
    typedef uint8_t pph_uint8_t;

    /* C99 literal suffixes */
    #define PPH_INT64_C(x)  (x##LL)
    #define PPH_UINT64_C(x) (x##ULL)

    /* GCC/Clang inline */
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #define PPH_INLINE inline
    #elif defined(__GNUC__)
        #define PPH_INLINE __inline__
    #else
        #define PPH_INLINE static
    #endif

#else
    /* Unknown compiler: Try C99 fallback */
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #include <stdint.h>
        typedef int64_t pph_int64_t;
        typedef uint64_t pph_uint64_t;
        typedef int32_t pph_int32_t;
        typedef uint32_t pph_uint32_t;
        typedef int16_t pph_int16_t;
        typedef uint16_t pph_uint16_t;
        typedef int8_t pph_int8_t;
        typedef uint8_t pph_uint8_t;
        #define PPH_INT64_C(x) (x##LL)
        #define PPH_INLINE inline
    #else
        /* Last resort: assume long long exists */
        typedef long long pph_int64_t;
        typedef unsigned long long pph_uint64_t;
        typedef long pph_int32_t;
        typedef unsigned long pph_uint32_t;
        typedef short pph_int16_t;
        typedef unsigned short pph_uint16_t;
        typedef signed char pph_int8_t;
        typedef unsigned char pph_uint8_t;
        #define PPH_INT64_C(x) (x##LL)
        #define PPH_INLINE static
    #endif
#endif

/* Size type */
#if defined(__WATCOMC__) || defined(_MSC_VER)
    typedef unsigned int pph_size_t;
#else
    #include <stddef.h>
    typedef size_t pph_size_t;
#endif

/* Printf format macros (for debugging) */
#if defined(__WATCOMC__) || defined(_MSC_VER)
    /* Both use Microsoft-style format strings */
    #define PPH_PRId64 "I64d"
    #define PPH_PRIu64 "I64u"
    #define PPH_PRIx64 "I64x"
#else
    /* GCC/Clang use C99 format strings */
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #include <inttypes.h>
        #define PPH_PRId64 PRId64
        #define PPH_PRIu64 PRIu64
        #define PPH_PRIx64 PRIx64
    #else
        #define PPH_PRId64 "lld"
        #define PPH_PRIu64 "llu"
        #define PPH_PRIx64 "llx"
    #endif
#endif

#endif /* PPH_TYPES_H */
