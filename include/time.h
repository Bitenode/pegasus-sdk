#ifndef PEGASUS_TIME_H
#define PEGASUS_TIME_H

#include <stddef.h>
#include <sys/types.h>

typedef long clock_t;

#define CLOCKS_PER_SEC 1000000L

typedef int clockid_t;
#define CLOCK_REALTIME  0
#define CLOCK_MONOTONIC 1

#ifndef _PEGASUS_STRUCT_TIMESPEC
#define _PEGASUS_STRUCT_TIMESPEC
struct timespec {
    time_t tv_sec;
    long   tv_nsec;
};
#endif

struct tm {
    int tm_sec;    /* [0,60] */
    int tm_min;    /* [0,59] */
    int tm_hour;   /* [0,23] */
    int tm_mday;   /* [1,31] */
    int tm_mon;    /* [0,11] */
    int tm_year;   /* years since 1900 */
    int tm_wday;   /* [0,6] (Sunday = 0) */
    int tm_yday;   /* [0,365] */
    int tm_isdst;
};

#ifdef __cplusplus
extern "C" {
#endif

time_t time(time_t *t);
double difftime(time_t end, time_t start);
clock_t clock(void);
int clock_gettime(clockid_t clk_id, struct timespec *tp);

struct tm *gmtime(const time_t *timep);
struct tm *gmtime_r(const time_t *timep, struct tm *result);
struct tm *localtime(const time_t *timep);
struct tm *localtime_r(const time_t *timep, struct tm *result);
time_t mktime(struct tm *tm);
time_t timegm(struct tm *tm);

char *asctime(const struct tm *tm);
char *asctime_r(const struct tm *tm, char *buf);
char *ctime(const time_t *timep);
char *ctime_r(const time_t *timep, char *buf);
size_t strftime(char *s, size_t max, const char *fmt, const struct tm *tm);

#ifdef __cplusplus
}
#endif

#endif
