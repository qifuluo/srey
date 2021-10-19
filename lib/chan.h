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
* \param   icapacity ����0 �����������
*/
void chan_init(struct chan_ctx *pctx, const int32_t icapacity);
/*
* \param   �ͷ�
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
int32_t chan_isclosed(struct chan_ctx *pctx);
/*
* \brief          д������
* \param pdata    ��д�������
* \return         ERR_OK �ɹ�
*/
int32_t chan_send(struct chan_ctx *pctx, void *pdata);
/*
* \brief          ��ȡ����
* \param pdata    ��ȡ��������
* \return         ERR_OK �ɹ�
*/
int32_t chan_recv(struct chan_ctx *pctx, void **pdata);
/*
* \brief          ��������
* \return         ��������
*/
int32_t chan_size(struct chan_ctx *pctx);
/*
* \brief          �Ƿ�ɶ�
* \return         ERR_OK ������
*/
int32_t chan_canrecv(struct chan_ctx *pctx);
/*
* \brief          �Ƿ��д
* \return         ERR_OK ��д
*/
int32_t chan_cansend(struct chan_ctx *pctx);
/*
* \brief             ���ѡȡһ�ɶ�д��channel��ִ�ж�д,�����Ĳ�֧��
* \param precv       ��cchan
* \param irecv_count ��cchan����
* \param precv_out   ����������
* \param psend       дcchan
* \param isend_count дcchan����
* \param psend_msgs  ÿ��cchan��Ҫ���͵�����
* \return            ERR_FAILED ʧ��
* \return            cchan �±�
*/
int32_t chan_select(struct chan_ctx *precv[], const int32_t irecv_count, void **precv_out,
    struct chan_ctx *psend[], const int32_t isend_count, void *psend_msgs[]);

#endif//CHAN_H_
