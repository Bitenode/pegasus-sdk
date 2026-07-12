#ifndef PEGASUS_UNISTD_H
#define PEGASUS_UNISTD_H

#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

ssize_t read(int fd, void *buf, size_t len);
ssize_t write(int fd, const void *buf, size_t len);
/* Variadic to match POSIX open() and libstdc++ <filesystem>'s 3-argument calls
 * (the mode argument is only meaningful with O_CREAT). Also declared in
 * <fcntl.h>; the two declarations are compatible. */
int open(const char *path, int flags, ...);
int close(int fd);
off_t lseek(int fd, off_t offset, int whence);

void _exit(int code) __attribute__((noreturn));
int yield(void);
pid_t getpid(void);
pid_t gettid(void);
int execve(const char *path, char *const argv[], char *const envp[]);
int spawn(const char *path, char *const argv[]);
void *sbrk(ptrdiff_t increment);
int usleep(unsigned long ticks);
int waitpid(int pid, int *status);
int kill(int pid);
int thread_create(void (*entry)(void), void *stack_top);
void thread_exit(int code) __attribute__((noreturn));

/* futex_wait: block until the 32-bit word at *uaddr no longer equals
 * `expected`, or a matching futex_wake arrives. Returns 0 on wake, or -1 with
 * errno=EAGAIN if *uaddr != expected at entry (no block). Spurious wakeups are
 * possible, so callers must re-test their condition in a loop.
 * futex_wake: wake up to `count` waiters on *uaddr; returns the number woken. */
int futex_wait(int *uaddr, int expected);
int futex_wake(int *uaddr, int count);
void *mmap(void *addr, size_t len);
int munmap(void *addr, size_t len);
int unlink(const char *path);
int mkdir(const char *path);
int rmdir(const char *path);
int rename(const char *old_path, const char *new_path);
int pipe(int pipefd[2]);
int dup2(int oldfd, int newfd);
int dup(int oldfd);
int isatty(int fd);
int chdir(const char *path);
char *getcwd(char *buf, size_t size);
unsigned int getcaps(void);

/* Symlink/hardlink surface Pegasus does not implement. Present so cross-built
 * libstdc++ <filesystem> links; they fail with errno=ENOSYS. */
int     symlink(const char *target, const char *linkpath);
int     link(const char *oldpath, const char *newpath);
ssize_t readlink(const char *path, char *buf, size_t bufsiz);

#ifdef __cplusplus
}
#endif

#endif
