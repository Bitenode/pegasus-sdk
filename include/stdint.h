#ifndef PEGASUS_STDINT_H
#define PEGASUS_STDINT_H

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long int64_t;
typedef unsigned long uint64_t;
typedef long intptr_t;
typedef unsigned long uintptr_t;

typedef long          intmax_t;
typedef unsigned long uintmax_t;

/* Minimum-width types (least). Exact-width types satisfy the minimums. */
typedef int8_t   int_least8_t;
typedef uint8_t  uint_least8_t;
typedef int16_t  int_least16_t;
typedef uint16_t uint_least16_t;
typedef int32_t  int_least32_t;
typedef uint32_t uint_least32_t;
typedef int64_t  int_least64_t;
typedef uint64_t uint_least64_t;

/* Fastest minimum-width types (LP64: 16/32/64 fast types map to 64-bit). */
typedef int8_t   int_fast8_t;
typedef uint8_t  uint_fast8_t;
typedef int64_t  int_fast16_t;
typedef uint64_t uint_fast16_t;
typedef int64_t  int_fast32_t;
typedef uint64_t uint_fast32_t;
typedef int64_t  int_fast64_t;
typedef uint64_t uint_fast64_t;

/* Exact-width integer limits (LP64: long is 64-bit). */
#define INT8_MAX    0x7f
#define INT8_MIN    (-INT8_MAX - 1)
#define UINT8_MAX   0xff
#define INT16_MAX   0x7fff
#define INT16_MIN   (-INT16_MAX - 1)
#define UINT16_MAX  0xffff
#define INT32_MAX   0x7fffffff
#define INT32_MIN   (-INT32_MAX - 1)
#define UINT32_MAX  0xffffffffU
#define INT64_MAX   0x7fffffffffffffffL
#define INT64_MIN   (-INT64_MAX - 1L)
#define UINT64_MAX  0xffffffffffffffffUL

#define INTPTR_MAX  INT64_MAX
#define INTPTR_MIN  INT64_MIN
#define UINTPTR_MAX UINT64_MAX
#define INTMAX_MAX  INT64_MAX
#define INTMAX_MIN  INT64_MIN
#define UINTMAX_MAX UINT64_MAX
#define SIZE_MAX    UINT64_MAX
#define PTRDIFF_MAX INT64_MAX
#define PTRDIFF_MIN INT64_MIN

/* least-width limits (exact-width backed) */
#define INT_LEAST8_MAX   INT8_MAX
#define INT_LEAST8_MIN   INT8_MIN
#define UINT_LEAST8_MAX  UINT8_MAX
#define INT_LEAST16_MAX  INT16_MAX
#define INT_LEAST16_MIN  INT16_MIN
#define UINT_LEAST16_MAX UINT16_MAX
#define INT_LEAST32_MAX  INT32_MAX
#define INT_LEAST32_MIN  INT32_MIN
#define UINT_LEAST32_MAX UINT32_MAX
#define INT_LEAST64_MAX  INT64_MAX
#define INT_LEAST64_MIN  INT64_MIN
#define UINT_LEAST64_MAX UINT64_MAX

/* fast-width limits (LP64: 16/32/64 fast types are 64-bit) */
#define INT_FAST8_MAX    INT8_MAX
#define INT_FAST8_MIN    INT8_MIN
#define UINT_FAST8_MAX   UINT8_MAX
#define INT_FAST16_MAX   INT64_MAX
#define INT_FAST16_MIN   INT64_MIN
#define UINT_FAST16_MAX  UINT64_MAX
#define INT_FAST32_MAX   INT64_MAX
#define INT_FAST32_MIN   INT64_MIN
#define UINT_FAST32_MAX  UINT64_MAX
#define INT_FAST64_MAX   INT64_MAX
#define INT_FAST64_MIN   INT64_MIN
#define UINT_FAST64_MAX  UINT64_MAX

/* Macros for constructing integer constants of a given width (C99 7.18.4). */
#define INT8_C(x)    (x)
#define INT16_C(x)   (x)
#define INT32_C(x)   (x)
#define INT64_C(x)   (x##L)
#define UINT8_C(x)   (x)
#define UINT16_C(x)  (x)
#define UINT32_C(x)  (x##U)
#define UINT64_C(x)  (x##UL)
#define INTMAX_C(x)  (x##L)
#define UINTMAX_C(x) (x##UL)

#endif
