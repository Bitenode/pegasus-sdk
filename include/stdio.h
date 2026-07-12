#ifndef PEGASUS_STDIO_H
#define PEGASUS_STDIO_H

#include <stddef.h>
#include <stdarg.h>

#ifndef EOF
#define EOF (-1)
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

#define BUFSIZ    1024
#define _IOFBF    0
#define _IOLBF    1
#define _IONBF    2
#define FOPEN_MAX 32
#define L_tmpnam  32

typedef struct _pegasus_file FILE;

/* File position type for fgetpos/fsetpos (opaque byte offset). */
typedef long fpos_t;

/* C linkage so C++ TUs (libstdc++) resolve libc's C symbols and redeclarations
 * with 'C' linkage (e.g. in cstdio) do not conflict. */
#ifdef __cplusplus
extern "C" {
#endif

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int putchar(int ch);
int puts(const char *s);
int fputc(int ch, FILE *stream);
int fputs(const char *s, FILE *stream);
int getchar(void);
char *fgets(char *s, int size, FILE *stream);
void perror(const char *s);

/*
 * Buffered-file API. Implemented by the Lua port (userspace/lua/port); these
 * prototypes complete <stdio.h> for programs that need FILE* I/O. The core
 * coreutils do not reference them, so libc.a itself carries no dependency on
 * these definitions.
 */
FILE *fopen(const char *path, const char *mode);
FILE *freopen(const char *path, const char *mode, FILE *stream);
FILE *fdopen(int fd, const char *mode);
int   fileno(FILE *stream);
int   fclose(FILE *stream);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int   fseek(FILE *stream, long offset, int whence);
long  ftell(FILE *stream);
int   fgetpos(FILE *stream, fpos_t *pos);
int   fsetpos(FILE *stream, const fpos_t *pos);
void  setbuf(FILE *stream, char *buf);
void  rewind(FILE *stream);
int   fflush(FILE *stream);
int   feof(FILE *stream);
int   ferror(FILE *stream);
void  clearerr(FILE *stream);
int   setvbuf(FILE *stream, char *buf, int mode, size_t size);
int   fgetc(FILE *stream);
int   getc(FILE *stream);
int   ungetc(int c, FILE *stream);
int   putc(int ch, FILE *stream);
int   remove(const char *path);
int   rename(const char *old_path, const char *new_path);
FILE *tmpfile(void);
char *tmpnam(char *s);

int printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int fprintf(FILE *stream, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int sprintf(char *buf, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int snprintf(char *buf, size_t size, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
int vprintf(const char *fmt, va_list ap);
int vfprintf(FILE *stream, const char *fmt, va_list ap);
int vsprintf(char *buf, const char *fmt, va_list ap);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);

int sscanf(const char *str, const char *fmt, ...) __attribute__((format(scanf, 2, 3)));
int vsscanf(const char *str, const char *fmt, va_list ap);
int scanf(const char *fmt, ...) __attribute__((format(scanf, 1, 2)));
int fscanf(FILE *stream, const char *fmt, ...) __attribute__((format(scanf, 2, 3)));

#ifdef __cplusplus
}
#endif

#endif
