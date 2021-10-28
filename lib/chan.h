#ifndef CHAN_H_
#define CHAN_H_

#include "queue.h"
#include "mutex.h"
#include "cond.h"

typedef struct chan_ctx
{
    mutex_ctx rmutex;//����
    mutex_ctx wmutex;//д��    
    mutex_ctx mmutex;//��д�ź���
    cond_ctx rcond;
    cond_ctx wcond;
    int32_t closed;
    int32_t rwaiting;
    int32_t wwaiting;
    struct queue_ctx *queue;
    void *data;
}chan_ctx;
/*
* \brief   ��ʼ��
* \param   icapacity ����0 �����������
*/
void chan_init(struct chan_ctx *pctx, const int32_t icapacity);
/*
* \brief   �ͷ�
*/
void chan_free(struct chan_ctx *pctx);

/*
* \brief  �ر�channel���رպ���д��
*/
void chan_close(struct chan_ctx *pctx);
/*
* \brief          channel�Ƿ�ر�
* \return         0 δ�ر�
*/
static inline int32_t chan_isclosed(struct chan_ctx *pctx)
{
    mutex_lock(&pctx->mmutex);
    int32_t iclosed = pctx->closed;
    mutex_unlock(&pctx->mmutex);

    return iclosed;
};
int32_t _bufferedsend(struct chan_ctx *pctx, void *pdata);
int32_t _bufferedrecv(struct chan_ctx *pctx, void **pdata);
int32_t _unbufferedsend(struct chan_ctx *pctx, void* pdata);
int32_t _unbufferedrecv(struct chan_ctx *pctx, void **pdata);
/*
* \brief          д������
* \param pdata    ��д�������
* \return         ERR_OK �ɹ�
*/
static inline int32_t chan_send(struct chan_ctx *pctx, void *pdata)
{
    if (0 != chan_isclosed(pctx))
    {
        return ERR_FAILED;
    }
    return NULL != pctx->queue ? _bufferedsend(pctx, pdata) : _unbufferedsend(pctx, pdata);
};
/*
* \brief          ��ȡ����
* \param pdata    ��ȡ��������
* \return         ERR_OK �ɹ�
*/
static inline int32_t chan_recv(struct chan_ctx *pctx, void **pdata)
{
    return NULL != pctx->queue ? _bufferedrecv(pctx, pdata) : _unbufferedrecv(pctx, pdata);
};
/*
* \brief          ��������
* \return         ��������
*/
static inline int32_t chan_size(struct chan_ctx *pctx)
{
    int32_t isize = 0;
    if (NULL != pctx->queue)
    {
        mutex_lock(&pctx->mmutex);
        isize = queue_size(pctx->queue);
        mutex_unlock(&pctx->mmutex);
    }
    return isize;
};
/*
* \brief          �Ƿ�ɶ�
* \return         ERR_OK ������
*/
static inline int32_t chan_canrecv(struct chan_ctx *pctx)
{
    if (NULL != pctx->queue)
    {
        return  chan_size(pctx) > 0 ? ERR_OK : ERR_FAILED;
    }

    mutex_lock(&pctx->mmutex);
    int32_t icanrecv = pctx->wwaiting > 0 ? ERR_OK : ERR_FAILED;
    mutex_unlock(&pctx->mmutex);

    return icanrecv;
};
/*
* \brief          �Ƿ��д
* \return         ERR_OK ��д
*/
static inline int32_t chan_cansend(struct chan_ctx *pctx)
{
    int32_t isend;
    if (NULL != pctx->queue)
    {
        mutex_lock(&pctx->mmutex);
        isend = queue_size(pctx->queue) < queue_cap(pctx->queue) ? ERR_OK : ERR_FAILED;
        mutex_unlock(&pctx->mmutex);
    }
    else
    {
        mutex_lock(&pctx->mmutex);
        isend = pctx->rwaiting > 0 ? ERR_OK : ERR_FAILED;
        mutex_unlock(&pctx->mmutex);
    }

    return isend;
};
/*
* \brief             ���ѡȡһ�ɶ�д��channel��ִ�ж�д,�����Ĳ�֧��
* \param precv       ��cchan
* \param irecvcnt    ��cchan����
* \param precv_out   ����������
* \param psend       дcchan
* \param isendcnt    дcchan����
* \param psend_msgs  ÿ��cchan��Ҫ���͵�����
* \return            ERR_FAILED ʧ��
* \return            cchan �±�
*/
int32_t chan_select(struct chan_ctx *precv[], const int32_t irecvcnt, void **precv_out,
    struct chan_ctx *psend[], const int32_t isendcnt, void *psend_msgs[]);

#endif//CHAN_H_
