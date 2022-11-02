#ifndef TIMER_H_
#define TIMER_H_

#include "macro.h"

struct timer_ctx
{
#if defined(OS_WIN)
    double interval;
#elif defined(OS_DARWIN) 
    double interval;
    uint64_t(*timefunc)(void);
#else
#endif
    uint64_t starttick;
};

/*
* \brief          ��ʼ��
*/
void timer_init(struct timer_ctx *pctx);
/*
* \brief          ��ǰʱ��
*/
uint64_t timer_cur(struct timer_ctx *pctx);
/*
* \brief          ��ʼ��ʱ
*/
void timer_start(struct timer_ctx *pctx);
/*
* \brief          ������ʱ
* \return         ��ʱ ����
*/
uint64_t timer_elapsed(struct timer_ctx *pctx);

#endif//TIMER_H_
