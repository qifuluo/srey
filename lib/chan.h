#ifndef CHAN_H_
#define CHAN_H_

#include "queue.h"
#include "mutex.h"
#include "cond.h"
#include "structs.h"

QUEUE_DECL(struct message_ctx, qu_chanmsg);
struct chan_ctx
{
    int32_t closed;
    int32_t rwaiting;
    int32_t wwaiting;
    mutex_ctx mmutex;//��д�ź���
    cond_ctx rcond;
    cond_ctx wcond;
    qu_chanmsg queue;
};
/*
* \brief             ��ʼ��
* \param icapacity   ��������
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
* \brief          д������
* \param pdata    ��д�������
* \return         ERR_OK �ɹ�
*/
int32_t chan_send(struct chan_ctx *pctx, struct message_ctx *pdata);
int32_t chan_trysend(struct chan_ctx *pctx, struct message_ctx *pdata);
/*
* \brief          ��ȡ����
* \param pdata    ��ȡ��������
* \return         ERR_OK �ɹ�
*/
int32_t chan_recv(struct chan_ctx *pctx, struct message_ctx *pdata);
int32_t chan_tryrecv(struct chan_ctx *pctx, struct message_ctx *pdata);

/*
* \brief          ��������
* \return         ��������
*/
int32_t chan_size(struct chan_ctx *pctx);
/*
* \brief          channel�Ƿ�ر�
* \return         0 δ�ر�
*/
int32_t chan_closed(struct chan_ctx *pctx);

#endif//CHAN_H_
