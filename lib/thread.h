#ifndef THREAD_H_
#define THREAD_H_

#include "macro.h"

#if defined(OS_WIN)
typedef HANDLE pthread_t;
#endif
struct thread_ctx
{
    volatile int32_t state;
    pthread_t pthread;
    void *data;
    void(*th_cb)(void*);
};

/*
* \brief             ��ʼ��
*/
void thread_init(struct thread_ctx *pctx);
/*
* \brief             ����һ�߳�,���ε���
* \param thread_cb   �ص�����
* \param pparam      �ص���������,������������ʱ��
*/
void thread_creat(struct thread_ctx *pctx, void(*cb)(void*), void *pdata);
/*
* \brief          �ȴ��߳�����
*/
void thread_wait(struct thread_ctx *pctx);
/*
* \brief          �ȴ��߳̽���
*/
void thread_join(struct thread_ctx *pctx);

#endif//THREAD_H_
