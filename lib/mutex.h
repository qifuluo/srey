#ifndef MUTEX_H_
#define MUTEX_H_

#include "macro.h"

#if defined(OS_WIN)
typedef CRITICAL_SECTION mutex_ctx;
#else
typedef pthread_mutex_t mutex_ctx;
#endif
/*
* \brief          ��ʼ��
*/
static inline void mutex_init(mutex_ctx *pctx)
{
#if defined(OS_WIN)
    InitializeCriticalSection(pctx);
#else
    ASSERTAB(ERR_OK == pthread_mutex_init(pctx, (const pthread_mutexattr_t*)NULL),
        ERRORSTR(ERRNO));
#endif
};
/*
* \brief          �ͷ�
*/
static inline void mutex_free(mutex_ctx *pctx)
{
#if defined(OS_WIN)
    DeleteCriticalSection(pctx);
#else
    (void)pthread_mutex_destroy(pctx);
#endif
};
/*
* \brief          ����
*/
static inline void mutex_lock(mutex_ctx *pctx)
{
#if defined(OS_WIN)
    EnterCriticalSection(pctx);
#else
    ASSERTAB(ERR_OK == pthread_mutex_lock(pctx), ERRORSTR(ERRNO));
#endif
};
/*
* \brief          try����
* \return         ERR_OK �ɹ�
*/
static inline int32_t mutex_trylock(mutex_ctx *pctx)
{
#if defined(OS_WIN)
    return TRUE == TryEnterCriticalSection(pctx) ? ERR_OK : ERR_FAILED;
#else
    return pthread_mutex_trylock(pctx);
#endif
};
/*
* \brief          ����
*/
static inline void  mutex_unlock(mutex_ctx *pctx)
{
#if defined(OS_WIN)
    LeaveCriticalSection(pctx);
#else
    ASSERTAB(ERR_OK == pthread_mutex_unlock(pctx), ERRORSTR(ERRNO));
#endif
};

#endif//MUTEX_H_
