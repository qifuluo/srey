#ifndef RWLOCK_H_
#define RWLOCK_H_

#include "macro.h"

struct rwlock_ctx
{
#if defined(OS_WIN)
    uint32_t wlock;
    SRWLOCK rwlock;
#else
    pthread_rwlock_t rwlock;
#endif
};
/*
* \brief          ��ʼ��
*/
void rwlock_init(struct rwlock_ctx *pctx);
/*
* \brief          �ͷ�
*/
void rwlock_free(struct rwlock_ctx *pctx);

/*
* \brief          ������
*/
void rwlock_rdlock(struct rwlock_ctx *pctx);
/*
* \brief          ������������
* \return         ERR_OK �ɹ�
*/
int32_t rwlock_tryrdlock(struct rwlock_ctx *pctx);
/*
* \brief          д����
* \return         true �ɹ�
*/
void rwlock_wrlock(struct rwlock_ctx *pctx);
/*
* \brief          ������д����
* \return         ERR_OK �ɹ�
*/
int32_t rwlock_trywrlock(struct rwlock_ctx *pctx);
/*
* \brief          ����
*/
void rwlock_unlock(struct rwlock_ctx *pctx);

#endif//RWLOCK_H_
