#ifndef PEGASUS_MATH_H
#define PEGASUS_MATH_H

/*
 * Hardware floating-point <math.h> for Pegasus userspace.
 *
 * Since P0 the kernel enables SSE/x87 and userspace builds with the standard
 * x86-64 ABI (float/double in XMM), so these are ordinary hardware-FP routines.
 * Implementations live in userspace/libm (libm.a). Programs that use them must
 * link libm.a in addition to libc.a.
 *
 * The Lua port keeps its own soft-float <math.h> (userspace/lua/include/math.h)
 * and does NOT use this header (its -I lua/include comes first); do not link
 * libm.a into the -msoft-float Lua binary.
 */

#ifdef __cplusplus
extern "C" {
#endif

#define HUGE_VAL   (__builtin_huge_val())
#define HUGE_VALF  (__builtin_huge_valf())
#define HUGE_VALL  (__builtin_huge_vall())
#define INFINITY   (__builtin_inff())
#define NAN        (__builtin_nanf(""))

#define M_E        2.7182818284590452354
#define M_LOG2E    1.4426950408889634074
#define M_LOG10E   0.43429448190325182765
#define M_LN2      0.69314718055994530942
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.78539816339744830962
#define M_1_PI     0.31830988618379067154
#define M_2_PI     0.63661977236758134308
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.70710678118654752440

/* Classification: use compiler builtins so these work without linking libm. */
#define FP_NAN       0
#define FP_INFINITE  1
#define FP_ZERO      2
#define FP_SUBNORMAL 3
#define FP_NORMAL    4

#define fpclassify(x) \
    __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, (x))
#define isnan(x)    __builtin_isnan(x)
#define isinf(x)    __builtin_isinf(x)
#define isfinite(x) __builtin_isfinite(x)
#define isnormal(x) __builtin_isnormal(x)
#define signbit(x)  __builtin_signbit(x)

#define isgreater(x, y)      __builtin_isgreater((x), (y))
#define isgreaterequal(x, y) __builtin_isgreaterequal((x), (y))
#define isless(x, y)         __builtin_isless((x), (y))
#define islessequal(x, y)    __builtin_islessequal((x), (y))
#define islessgreater(x, y)  __builtin_islessgreater((x), (y))
#define isunordered(x, y)    __builtin_isunordered((x), (y))

#define MATH_ERRNO     1
#define MATH_ERREXCEPT 2
#define math_errhandling MATH_ERRNO

/* ---- double ---- */
double fabs(double x);
double sqrt(double x);
double cbrt(double x);
double hypot(double x, double y);
double pow(double x, double y);
double fmod(double x, double y);
double floor(double x);
double ceil(double x);
double trunc(double x);
double round(double x);
double nearbyint(double x);
double rint(double x);
double copysign(double x, double y);
double nan(const char *tag);
double exp(double x);
double exp2(double x);
double expm1(double x);
double log(double x);
double log2(double x);
double log10(double x);
double log1p(double x);
double sin(double x);
double cos(double x);
double tan(double x);
double asin(double x);
double acos(double x);
double atan(double x);
double atan2(double y, double x);
double sinh(double x);
double cosh(double x);
double tanh(double x);
double asinh(double x);
double acosh(double x);
double atanh(double x);
double ldexp(double x, int exp);
double frexp(double x, int *exp);
double modf(double x, double *iptr);
double scalbn(double x, int exp);
double fmin(double x, double y);
double fmax(double x, double y);
double fdim(double x, double y);
double fma(double x, double y, double z);
double tgamma(double x);
double lgamma(double x);
double erf(double x);
double erfc(double x);
long   lround(double x);
long long llround(double x);
long   lrint(double x);
long long llrint(double x);

/* ---- float (thin wrappers over double) ---- */
float fabsf(float x);
float sqrtf(float x);
float powf(float x, float y);
float fmodf(float x, float y);
float floorf(float x);
float ceilf(float x);
float truncf(float x);
float roundf(float x);
float copysignf(float x, float y);
float expf(float x);
float logf(float x);
float log2f(float x);
float log10f(float x);
float sinf(float x);
float cosf(float x);
float tanf(float x);
float atanf(float x);
float atan2f(float y, float x);
float asinf(float x);
float acosf(float x);
float ldexpf(float x, int exp);
float frexpf(float x, int *exp);
float modff(float x, float *iptr);
float scalbnf(float x, int exp);
float fminf(float x, float y);
float fmaxf(float x, float y);
float hypotf(float x, float y);
float cbrtf(float x);
float nearbyintf(float x);
float rintf(float x);
float nanf(const char *tag);
float exp2f(float x);
float expm1f(float x);
float log1pf(float x);
float sinhf(float x);
float coshf(float x);
float tanhf(float x);
float asinhf(float x);
float acoshf(float x);
float atanhf(float x);
float fdimf(float x, float y);
float fmaf(float x, float y, float z);
float tgammaf(float x);
float lgammaf(float x);
float erff(float x);
float erfcf(float x);
long      lroundf(float x);
long long llroundf(float x);
long      lrintf(float x);
long long llrintf(float x);

/* ---- long double (== double on this ABI) ---- */
long double fabsl(long double x);
long double sqrtl(long double x);
long double powl(long double x, long double y);
long double fmodl(long double x, long double y);
long double floorl(long double x);
long double ceill(long double x);
long double truncl(long double x);
long double roundl(long double x);
long double copysignl(long double x, long double y);
long double expl(long double x);
long double logl(long double x);
long double sinl(long double x);
long double cosl(long double x);
long double ldexpl(long double x, int exp);
long double frexpl(long double x, int *exp);
long double scalbnl(long double x, int exp);
long double modfl(long double x, long double *iptr);
long double cbrtl(long double x);
long double hypotl(long double x, long double y);
long double tanl(long double x);
long double asinl(long double x);
long double acosl(long double x);
long double atanl(long double x);
long double atan2l(long double y, long double x);
long double sinhl(long double x);
long double coshl(long double x);
long double tanhl(long double x);
long double asinhl(long double x);
long double acoshl(long double x);
long double atanhl(long double x);
long double exp2l(long double x);
long double expm1l(long double x);
long double log2l(long double x);
long double log10l(long double x);
long double log1pl(long double x);
long double fminl(long double x, long double y);
long double fmaxl(long double x, long double y);
long double fdiml(long double x, long double y);
long double fmal(long double x, long double y, long double z);
long double tgammal(long double x);
long double lgammal(long double x);
long double erfl(long double x);
long double erfcl(long double x);
long double nearbyintl(long double x);
long double rintl(long double x);
long double nanl(const char *tag);
long      lroundl(long double x);
long long llroundl(long double x);
long      lrintl(long double x);
long long llrintl(long double x);

#ifdef __cplusplus
}
#endif

#endif
