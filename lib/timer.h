#ifndef TIMER_H_
#define TIMER_H_

#include "macro.h"

typedef struct timer_ctx
{
#if defined(OS_WIN)
    double interval;
#elif defined(OS_DARWIN) 
    double interval;
    uint64_t(*timefunc)(void);
#else
#endif
    uint64_t starttick;
}timer_ctx;

/*
* \brief          ��ʼ��
*/
void timer_init(timer_ctx *ctx);
/*
* \brief          ��ǰʱ��
*/
uint64_t timer_cur(timer_ctx *ctx);
/*
* \brief          ��ʼ��ʱ
*/
void timer_start(timer_ctx *ctx);
/*
* \brief          ������ʱ
* \return         ��ʱ ����
*/
uint64_t timer_elapsed(timer_ctx *ctx);

#endif//TIMER_H_
