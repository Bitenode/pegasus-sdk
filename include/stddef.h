#ifndef PEGASUS_STDDEF_H
#define PEGASUS_STDDEF_H

typedef unsigned long size_t;
typedef long ptrdiff_t;

/* wchar_t is a built-in type in C++, but in C it must be provided by <stddef.h>
 * (and <stdlib.h>/<wchar.h>). Guarded so <wchar.h> and this header agree. */
#if !defined(__cplusplus) && !defined(_PEGASUS_WCHAR_T_DEFINED)
#define _PEGASUS_WCHAR_T_DEFINED
typedef int wchar_t;
#endif

/*
 * max_align_t: a type whose alignment is the greatest fundamental alignment.
 * Needed because the demos build with -nostdinc (GCC's own <stddef.h> is not on
 * the search path) yet GNU libstdc++'s <cstddef> does `using ::max_align_t;`.
 * Definition mirrors GCC's own stddef.h.
 */
#ifndef _GCC_MAX_ALIGN_T
#define _GCC_MAX_ALIGN_T
typedef struct {
  long long   __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
} max_align_t;
#endif

#ifndef NULL
#ifdef __cplusplus
/* In C++ a void* does not implicitly convert to T*, so NULL must not be
 * ((void*)0) — otherwise passing NULL to typed pointer parameters (e.g. gthreads
 * calling pthread_create(..., NULL, ...)) fails to compile. */
#define NULL __null
#else
#define NULL ((void *)0)
#endif
#endif

#ifndef offsetof
#define offsetof(type, member) __builtin_offsetof(type, member)
#endif

#endif
