#ifndef EASYRPG_SYMBIAN_LIBC_COMPAT_TIME_H
#define EASYRPG_SYMBIAN_LIBC_COMPAT_TIME_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef unsigned long clock_t;
    typedef int time_t;

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 128
#endif

    struct tm
    {
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;
        int tm_mon;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;
        long tm_gmtoff;
        char *tm_zone;
    };

    char *asctime(const struct tm *timeptr);
    clock_t clock(void);
    char *ctime(const time_t *timer);
    double difftime(time_t time1, time_t time0);
    struct tm *gmtime(const time_t *timer);
    struct tm *localtime(const time_t *timer);
    time_t mktime(struct tm *timeptr);
    size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *timeptr);
    time_t time(time_t *timer);

#ifdef __cplusplus
}
#endif

#endif
