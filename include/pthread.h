#ifndef PEGASUS_PTHREAD_H
#define PEGASUS_PTHREAD_H

/*
 * Minimal POSIX threads subset for Pegasus OS.
 *
 * This is the gthreads backing for GNU libstdc++ built --enable-threads=posix:
 * std::thread, std::mutex, std::condition_variable, std::call_once and
 * thread_local map onto these entry points. Everything is layered on top of the
 * kernel thread syscalls (SYS_THREAD_CREATE / SYS_THREAD_EXIT / SYS_GETTID) and
 * the SYS_FUTEX blocking primitive (see userspace/libc futex_wait/futex_wake) —
 * there is no separate kernel scheduler object per mutex/cond.
 *
 * Scope: the common surface libstdc++'s gthr-posix.h needs. It is intentionally
 * small (no cancellation, no realtime scheduling, no robust/recursive mutex
 * variants) but keeps standard signatures so the same headers satisfy the
 * cross-built libstdc++ in a provisioned toolchain host.
 */

#include <stddef.h>
#include <sched.h>   /* struct sched_param, sched_yield, sched_get_priority_* */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * struct timespec is only referenced by-pointer in the timed-wait prototypes
 * below. Forward-declaring it keeps this header self-contained; its full
 * definition lives in <time.h>. The timed/sched/cancel/rwlock prototypes exist
 * so GCC's gthr-posix.h wrappers (libgcc + libstdc++) compile; those entry
 * points are weak-referenced by gthreads and are not part of the implemented
 * Pegasus libpthread subset.
 */
struct timespec;

typedef unsigned long pthread_t;

typedef struct {
    size_t stacksize;
    int    detachstate;
} pthread_attr_t;

/* Futex-backed mutex, Drepper 3-state: 0=unlocked, 1=locked (no waiters),
 * 2=locked (possible waiters). The single int IS the futex word. */
typedef struct { int __state; } pthread_mutex_t;
typedef struct { int __kind; }  pthread_mutexattr_t;

/* Condition variable: a sequence counter used as the futex word. */
typedef struct { int __seq; }   pthread_cond_t;
typedef struct { int __unused; } pthread_condattr_t;

typedef int pthread_once_t;
typedef unsigned int pthread_key_t;

/* Read-write lock: single futex word (unimplemented subset; present so
 * gthr-posix.h's __gthread_rwlock_t typedef and shared_mutex compile). */
typedef struct { int __lock; }   pthread_rwlock_t;
typedef struct { int __unused; } pthread_rwlockattr_t;

#define PTHREAD_MUTEX_INITIALIZER { 0 }
#define PTHREAD_COND_INITIALIZER  { 0 }
#define PTHREAD_RWLOCK_INITIALIZER { 0 }
#define PTHREAD_ONCE_INIT         0

#define PTHREAD_CREATE_JOINABLE 0
#define PTHREAD_CREATE_DETACHED 1

#define PTHREAD_MUTEX_NORMAL     0
#define PTHREAD_MUTEX_RECURSIVE  1
#define PTHREAD_MUTEX_ERRORCHECK 2
#define PTHREAD_MUTEX_DEFAULT    PTHREAD_MUTEX_NORMAL

/* Threads */
int       pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                         void *(*start_routine)(void *), void *arg);
int       pthread_join(pthread_t thread, void **retval);
int       pthread_detach(pthread_t thread);
void      pthread_exit(void *retval) __attribute__((noreturn));
pthread_t pthread_self(void);
int       pthread_equal(pthread_t a, pthread_t b);

int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);

/* Mutexes */
int pthread_mutex_init(pthread_mutex_t *m, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *m);
int pthread_mutex_lock(pthread_mutex_t *m);
int pthread_mutex_trylock(pthread_mutex_t *m);
int pthread_mutex_unlock(pthread_mutex_t *m);

int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);

/* Condition variables */
int pthread_cond_init(pthread_cond_t *c, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *c);
int pthread_cond_wait(pthread_cond_t *c, pthread_mutex_t *m);
int pthread_cond_signal(pthread_cond_t *c);
int pthread_cond_broadcast(pthread_cond_t *c);

/* One-time initialization */
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));

/* Thread-specific data (thread_local / __cxa_thread_atexit backing) */
int   pthread_key_create(pthread_key_t *key, void (*destructor)(void *));
int   pthread_key_delete(pthread_key_t key);
void *pthread_getspecific(pthread_key_t key);
int   pthread_setspecific(pthread_key_t key, const void *value);

/*
 * Extended entry points referenced (weakly) by GCC's gthr-posix.h. They are
 * declared here only so libgcc/libstdc++ compile against this sysroot; they are
 * NOT implemented by the Pegasus libpthread subset. gthreads takes them as weak
 * references, so leaving them undefined is a link-time no-op unless a program
 * actually pulls one in (e.g. std::shared_mutex, std::condition_variable timed
 * waits, or thread scheduling/cancellation — none of which the demos use).
 */
int pthread_cond_timedwait(pthread_cond_t *c, pthread_mutex_t *m,
                           const struct timespec *abstime);
int pthread_mutex_timedlock(pthread_mutex_t *m, const struct timespec *abstime);
int pthread_getschedparam(pthread_t thread, int *policy,
                          struct sched_param *param);
int pthread_setschedparam(pthread_t thread, int policy,
                          const struct sched_param *param);
int pthread_cancel(pthread_t thread);

/* Read-write locks */
int pthread_rwlock_init(pthread_rwlock_t *rw, const pthread_rwlockattr_t *attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rw);
int pthread_rwlock_rdlock(pthread_rwlock_t *rw);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rw);
int pthread_rwlock_wrlock(pthread_rwlock_t *rw);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rw);
int pthread_rwlock_unlock(pthread_rwlock_t *rw);

#ifdef __cplusplus
}
#endif

#endif
