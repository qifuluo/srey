#ifndef TIMER_H_
#define TIMER_H_

#include "base/macro.h"

typedef struct timer_ctx {
#if defined(OS_WIN)
    double interval;
#elif defined(OS_DARWIN) 
    double interval;
    uint64_t(*timefunc)(void);
#else
#endif
    uint64_t starttick;
}timer_ctx;
/// <summary>
/// ��ʼ����ʱ��
/// </summary>
/// <param name="ctx">timer_ctx</param>
void timer_init(timer_ctx *ctx);
/// <summary>
/// ��ǰʱ��
/// </summary>
/// <param name="ctx">timer_ctx</param>
/// <returns>����</returns>
uint64_t timer_cur(timer_ctx *ctx);
/// <summary>
/// ��ǰʱ��
/// </summary>
/// <param name="ctx">timer_ctx</param>
/// <returns>����</returns>
uint64_t timer_cur_ms(timer_ctx *ctx);
/// <summary>
/// ��ʼ��ʱ
/// </summary>
/// <param name="ctx">timer_ctx</param>
void timer_start(timer_ctx *ctx);
/// <summary>
/// ��ʱ
/// </summary>
/// <param name="ctx">timer_ctx</param>
/// <returns>����</returns>
uint64_t timer_elapsed(timer_ctx *ctx);
/// <summary>
/// ��ʱ
/// </summary>
/// <param name="ctx">timer_ctx</param>
/// <returns>����</returns>
uint64_t timer_elapsed_ms(timer_ctx *ctx);

#endif//TIMER_H_
