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
void mutex_init(mutex_ctx *pctx);
/*
* \brief          �ͷ�
*/
void mutex_free(mutex_ctx *pctx);
/*
* \brief          ����
*/
void mutex_lock(mutex_ctx *pctx);
/*
* \brief          try����
* \return         ERR_OK �ɹ�
*/
int32_t mutex_trylock(mutex_ctx *pctx);
/*
* \brief          ����
*/
void mutex_unlock(mutex_ctx *pctx);

#endif//MUTEX_H_
