#include "timer.h"

#define NANOSEC 1000000000

void timer_init(struct timer_ctx *pctx)
{
#if defined(OS_WIN)
    pctx->interval = 0;
    LARGE_INTEGER freq;
    ASSERTAB(QueryPerformanceFrequency(&freq), ERRORSTR(ERRNO));
    pctx->interval = 1.0 / freq.QuadPart;
#elif defined(OS_DARWIN) 
    mach_timebase_info_data_t timebase;
    ASSERTAB(KERN_SUCCESS == mach_timebase_info(&timebase), "mach_timebase_info error.");
    pctx->interval = (double)timebase.numer / (double)timebase.denom;
    pctx->timefunc = (uint64_t(*)(void)) dlsym(RTLD_DEFAULT, "mach_continuous_time");
    if (NULL == pctx->timefunc)
    {
        pctx->timefunc = mach_absolute_time;
    }
#else
#endif
}
uint64_t timer_cur(struct timer_ctx *pctx)
{
#if defined(OS_WIN)
    LARGE_INTEGER lnow;
    ASSERTAB(QueryPerformanceCounter(&lnow), ERRORSTR(ERRNO));
    return (uint64_t)(lnow.QuadPart * pctx->interval * NANOSEC);
#elif defined(OS_AIX)
    timebasestruct_t t;
    read_wall_time(&t, TIMEBASE_SZ);
    time_base_to_time(&t, TIMEBASE_SZ);
    return (((uint64_t)t.tb_high) * NANOSEC + t.tb_low);
#elif defined(OS_SUN)
    return gethrtime();
#elif defined(OS_DARWIN)
    return (uint64_t)(pctx->timefunc() * pctx->interval);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (((uint64_t)ts.tv_sec) * NANOSEC + ts.tv_nsec);
#endif
}
void timer_start(struct timer_ctx *pctx)
{
    pctx->starttick = timer_cur(pctx);
}
uint64_t timer_elapsed(struct timer_ctx *pctx)
{
    return timer_cur(pctx) - pctx->starttick;
}
