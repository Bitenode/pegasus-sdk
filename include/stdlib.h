#ifndef PEGASUS_STDLIB_H
#define PEGASUS_STDLIB_H

#include <stddef.h>

/*
 * C linkage for the whole header: GNU libsupc++/libstdc++ redeclare several of
 * these (e.g. `extern "C" void *memalign(...)` in new_opa.cc) with C linkage, so
 * the libc declarations must also be extern "C" in C++ TUs to avoid
 * "conflicting declaration ... with 'C' linkage". Most standard names are GCC
 * builtins that already get C linkage, but non-builtins (memalign, aligned_alloc,
 * posix_memalign, mbstowcs, ...) do not — hence the explicit guard.
 */
#ifdef __cplusplus
extern "C" {
#endif

void *malloc(size_t size);
void  free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

void abort(void) __attribute__((noreturn));
void exit(int status) __attribute__((noreturn));

/*
 * Itanium C++ ABI static-storage destructor registration (userspace/libc/src/
 * cxa_atexit.c). C++ global objects register their destructors via
 * __cxa_atexit; crt0 calls __cxa_finalize(NULL) after main() to run them.
 */
int __cxa_atexit(void (*func)(void *), void *arg, void *dso);
void __cxa_finalize(void *dso);
extern void *__dso_handle;

char *getenv(const char *name);

long strtol(const char *nptr, char **endptr, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);
long long strtoll(const char *nptr, char **endptr, int base);
unsigned long long strtoull(const char *nptr, char **endptr, int base);
int atoi(const char *s);
long atol(const char *s);
long long atoll(const char *s);

/*
 * Hardware floating-point conversions (libc, since P0). strtod/strtof/strtold
 * live in their own translation unit (src/strtod.c) so the archive only pulls
 * them when referenced; the -msoft-float Lua port keeps its own strtod/atof
 * (userspace/lua/port/lua_fmt.c) and never links these.
 */
double      strtod(const char *nptr, char **endptr);
float       strtof(const char *nptr, char **endptr);
long double strtold(const char *nptr, char **endptr);
double atof(const char *nptr);

int       abs(int n);
long      labs(long n);
long long llabs(long long n);

typedef struct { int quot; int rem; } div_t;
typedef struct { long quot; long rem; } ldiv_t;
typedef struct { long long quot; long long rem; } lldiv_t;
div_t   div(int num, int den);
ldiv_t  ldiv(long num, long den);
lldiv_t lldiv(long long num, long long den);

void qsort(void *base, size_t nmemb, size_t size,
           int (*cmp)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t nmemb, size_t size,
              int (*cmp)(const void *, const void *));

void *aligned_alloc(size_t alignment, size_t size);
void *memalign(size_t alignment, size_t size);
int   posix_memalign(void **memptr, size_t alignment, size_t size);

int atexit(void (*fn)(void));

/* os.execute: direct ELF spawn via posix_spawn (see userspace/lua/port/lua_os.c). */
int system(const char *command);

/*
 * Pseudo-random numbers and multibyte conversions. Declared so GNU libstdc++'s
 * <cstdlib> `using ::rand;`, `using ::srand;`, `using ::mblen;` etc. resolve
 * (these are part of the non-optional <cstdlib> surface). The multibyte
 * conversions are also declared (identically) in <wchar.h> with C linkage, so
 * these MUST be wrapped in extern "C": without it they would take C++ linkage
 * in a C++ TU and clash with <wchar.h> ("conflicting declaration ... with 'C'
 * linkage"). Definitions live in libc (rand/srand) or are pulled from the
 * archive only if a program actually references them.
 */
int  rand(void);
void srand(unsigned int seed);

int    mblen(const char *s, size_t n);
int    mbtowc(wchar_t *pwc, const char *s, size_t n);
int    wctomb(char *s, wchar_t wc);
size_t mbstowcs(wchar_t *dst, const char *src, size_t n);
size_t wcstombs(char *dst, const wchar_t *src, size_t n);

extern char **environ;

#ifdef __cplusplus
}
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define RAND_MAX 2147483647

#endif
