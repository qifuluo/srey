#ifndef COND_H_
#define COND_H_

#include "mutex.h"

#if defined(OS_WIN)
typedef CONDITION_VARIABLE cond_ctx;
#else
typedef pthread_cond_t cond_ctx;
#endif
/*
* \brief          ��ʼ��
*/
void cond_init(cond_ctx *pctx);
/*
* \brief          �ͷ�
*/
void cond_free(cond_ctx *pctx);
/*
* \brief          �ȴ��ź���
*/
void cond_wait(cond_ctx *pctx, mutex_ctx *pmutex);
/*
* \brief          �ȴ��ź���
* \param uims     �ȴ�ʱ��  ����
*/
void cond_timedwait(cond_ctx *pctx, mutex_ctx *pmutex, const uint32_t uims);
/*
* \brief          �����ź�
*/
void cond_signal(cond_ctx *pctx);
/*
* \brief          ���������ź�
*/
void cond_broadcast(cond_ctx *pctx);

#endif//COND_H_
