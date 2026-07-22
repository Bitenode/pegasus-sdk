#ifndef PEGASUS_SIGNAL_H
#define PEGASUS_SIGNAL_H

/*
 * POSIX signals for Pegasus userspace. Backed by the kernel signal subsystem
 * (SYS_SIGACTION / SYS_SIGRETURN / SYS_SIGPROCMASK and kill(pid, signo)); see
 * docs/ABI.md. Signal numbers match kernel/include/process.h.
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int sig_atomic_t;
typedef unsigned long sigset_t;
typedef void (*__sighandler_t)(int);

#define SIG_DFL ((__sighandler_t)0)
#define SIG_IGN ((__sighandler_t)1)
#define SIG_ERR ((__sighandler_t)-1)

#define SIGHUP   1
#define SIGINT   2
#define SIGQUIT  3
#define SIGILL   4
#define SIGTRAP  5
#define SIGABRT  6
#define SIGBUS   7
#define SIGFPE   8
#define SIGKILL  9
#define SIGUSR1  10
#define SIGSEGV  11
#define SIGUSR2  12
#define SIGPIPE  13
#define SIGALRM  14
#define SIGTERM  15
#define SIGCHLD  17
#define SIGCONT  18
#define SIGSTOP  19
#define SIGTSTP  20
#define NSIG     32

/* sa_flags */
#define SA_NODEFER   0x40000000
#define SA_RESETHAND 0x80000000
#define SA_RESTART   0x10000000  /* accepted for source compat; not auto-restarting */

/* sigprocmask how */
#define SIG_BLOCK    0
#define SIG_UNBLOCK  1
#define SIG_SETMASK  2

struct sigaction {
    __sighandler_t sa_handler;
    sigset_t       sa_mask;
    int            sa_flags;
    void         (*sa_restorer)(void);
};

int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signo);
int sigdelset(sigset_t *set, int signo);
int sigismember(const sigset_t *set, int signo);

__sighandler_t signal(int signum, __sighandler_t handler);
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int raise(int sig);

/* Internal: the signal-return trampoline the kernel points a handler's return
 * address at. Not called directly by programs. */
void __sigreturn(void);

#ifdef __cplusplus
}
#endif

#endif
