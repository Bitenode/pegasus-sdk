#ifndef PEGASUS_WCHAR_H
#define PEGASUS_WCHAR_H

#include <stddef.h>
#include <stdarg.h>

#if !defined(__cplusplus) && !defined(_PEGASUS_WCHAR_T_DEFINED)
#define _PEGASUS_WCHAR_T_DEFINED
typedef int wchar_t;
#endif

typedef int wint_t;

#ifndef WEOF
#define WEOF ((wint_t)-1)
#endif

#ifndef WCHAR_MIN
#define WCHAR_MIN (-2147483647 - 1)
#define WCHAR_MAX 2147483647
#endif

typedef struct { int __count; unsigned int __value; } mbstate_t;

#ifdef __cplusplus
extern "C" {
#endif

/* wide memory / string */
wchar_t *wmemcpy(wchar_t *dst, const wchar_t *src, size_t n);
wchar_t *wmemmove(wchar_t *dst, const wchar_t *src, size_t n);
wchar_t *wmemset(wchar_t *dst, wchar_t c, size_t n);
int      wmemcmp(const wchar_t *a, const wchar_t *b, size_t n);
wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n);

size_t   wcslen(const wchar_t *s);
int      wcscmp(const wchar_t *a, const wchar_t *b);
int      wcsncmp(const wchar_t *a, const wchar_t *b, size_t n);
wchar_t *wcscpy(wchar_t *dst, const wchar_t *src);
wchar_t *wcsncpy(wchar_t *dst, const wchar_t *src, size_t n);
wchar_t *wcscat(wchar_t *dst, const wchar_t *src);
wchar_t *wcschr(const wchar_t *s, wchar_t c);
wchar_t *wcsrchr(const wchar_t *s, wchar_t c);

/* multibyte <-> wide (C locale: 1 byte == 1 wchar) */
size_t mbstowcs(wchar_t *dst, const char *src, size_t n);
size_t wcstombs(char *dst, const wchar_t *src, size_t n);
int    mbtowc(wchar_t *pwc, const char *s, size_t n);
int    wctomb(char *s, wchar_t wc);
int    mblen(const char *s, size_t n);
size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps);
size_t wcrtomb(char *s, wchar_t wc, mbstate_t *ps);
size_t mbrlen(const char *s, size_t n, mbstate_t *ps);
int    mbsinit(const mbstate_t *ps);
wint_t btowc(int c);
int    wctob(wint_t c);

#ifdef __cplusplus
}
#endif

#endif
