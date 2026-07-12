#ifndef PEGASUS_SCHED_H
#define PEGASUS_SCHED_H

/*
 * Minimal <sched.h> for Pegasus OS.
 *
 * GNU libstdc++/libgcc built --enable-threads=posix pull scheduling entry
 * points (sched_yield, sched_get_priority_min/max) and struct sched_param in
 * through <pthread.h>, exactly as glibc's <pthread.h> transitively includes
 * <sched.h>. Pegasus has no realtime scheduling, so these are a tiny stub: the
 * declarations exist so gthr-posix.h compiles, and gthreads references them
 * weakly (an unimplemented entry point is a link-time no-op unless a program
 * actually calls it — the STL demos do not).
 */

#ifdef __cplusplus
extern "C" {
#endif

struct sched_param {
    int sched_priority;
};

#define SCHED_OTHER 0
#define SCHED_FIFO  1
#define SCHED_RR    2

int sched_yield(void);
int sched_get_priority_max(int policy);
int sched_get_priority_min(int policy);

#ifdef __cplusplus
}
#endif

#endif /* PEGASUS_SCHED_H */
