/*
 * <assert.h> for Pegasus OS.
 *
 * Deliberately has no single include guard around the macro: per the C standard
 * assert() must be (re)defined on every inclusion according to the current
 * NDEBUG state. GNU libstdc++'s <cassert> requires this header to exist. A
 * failed assertion routes to abort() (already provided by libc), so no new
 * runtime symbol is introduced.
 */

#undef assert

#ifdef NDEBUG
# define assert(expr) ((void)0)
#else

# ifdef __cplusplus
extern "C" {
# endif
void abort(void) __attribute__((__noreturn__));
# ifdef __cplusplus
}
# endif

# define assert(expr) ((expr) ? (void)0 : abort())
#endif

#ifndef PEGASUS_ASSERT_STATIC_ASSERT
#define PEGASUS_ASSERT_STATIC_ASSERT
# if !defined(__cplusplus) && (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
#  define static_assert _Static_assert
# endif
#endif
