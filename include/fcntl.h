#ifndef PEGASUS_FCNTL_H
#define PEGASUS_FCNTL_H

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR   2
#define O_CREAT     0x0040
#define O_EXCL      0x0080
#define O_TRUNC     0x0200
#define O_APPEND    0x0400
#define O_NONBLOCK  0x0800
#define O_DIRECTORY 0x10000
#define O_NOFOLLOW  0x20000
#define O_CLOEXEC   0x80000

/* fcntl() commands + file-descriptor flags (used by libstdc++/libbacktrace). */
#define F_DUPFD 0
#define F_GETFD 1
#define F_SETFD 2
#define F_GETFL 3
#define F_SETFL 4
#define FD_CLOEXEC 1

/*
 * POSIX open() is variadic: the third argument (mode) is consumed only when
 * O_CREAT is set. GNU libstdc++'s <filesystem> calls the 3-argument form, so
 * this must not be a fixed 2-parameter prototype.
 */
#ifdef __cplusplus
extern "C" {
#endif
int open(const char *path, int flags, ...);
/* fcntl(): declared so libstdc++/libbacktrace compile. The Pegasus libc does not
 * implement it (no runtime symbol) — only referenced by stacktrace support,
 * which the STL demos do not link. */
int fcntl(int fd, int cmd, ...);
#ifdef __cplusplus
}
#endif

#endif
