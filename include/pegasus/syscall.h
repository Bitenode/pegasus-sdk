#ifndef PEGASUS_SYSCALL_H
#define PEGASUS_SYSCALL_H

#include <stdint.h>

/* Auto-generated from abi/syscalls.json by tools/gen_syscalls.py — do not edit. */

#define SYS_EXIT              0
#define SYS_READ              1
#define SYS_WRITE             2
#define SYS_OPEN              3
#define SYS_CLOSE             4
#define SYS_MMAP              5
#define SYS_MUNMAP            6
#define SYS_YIELD             7
#define SYS_GETPID            8
#define SYS_BRK               9
#define SYS_IOCTL             10
#define SYS_WAIT              11
#define SYS_KILL              12
#define SYS_GETCAPS           13
#define SYS_SLEEP             14
#define SYS_THREAD_CREATE     15
#define SYS_THREAD_EXIT       16
#define SYS_GETTID            17
#define SYS_LSEEK             18
#define SYS_EXECVE            19
#define SYS_SOCKET            20
#define SYS_CONNECT           21
#define SYS_SEND              22
#define SYS_RECV              23
#define SYS_STAT              24
#define SYS_FSTAT             25
#define SYS_GETTIMEOFDAY      26
#define SYS_GETDENTS          27
#define SYS_UNLINK            28
#define SYS_MKDIR             29
#define SYS_RENAME            30
#define SYS_PIPE              31
#define SYS_DUP2              32
#define SYS_CHDIR             33
#define SYS_GETCWD            34
#define SYS_RMDIR             35
#define SYS_FUTEX             36

#define SYSCALL_MAX       37

long syscall0(uint64_t num);
long syscall1(uint64_t num, uint64_t a0);
long syscall2(uint64_t num, uint64_t a0, uint64_t a1);
long syscall3(uint64_t num, uint64_t a0, uint64_t a1, uint64_t a2);
long syscall4(uint64_t num, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3);
long syscall5(uint64_t num, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4);

long syscall_ret(long ret);
long syscall_ret_ssize(long ret);
long syscall_ret_off(long ret);
void *syscall_ret_ptr(long ret);

#endif
