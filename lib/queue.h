#ifndef QUEUE_H_
#define QUEUE_H_

#include "structs.h"

struct queue_ctx
{
    int32_t size;
    int32_t next;
    int32_t capacity;
    int32_t initcap;
    struct message_ctx *msg;
};

/*
* \brief          ��ʼ��
*/
void queue_init(struct queue_ctx *pctx, const int32_t icapacity);
/*
* \brief          �ͷ�
*/
void queue_free(struct queue_ctx *pctx);
/*
* \brief          ����
*/
void queue_expand(struct queue_ctx *pctx);
/*
* \brief          �������
* \param pval     ��Ҫ��ӵ�����
* \return         ERR_OK �ɹ�
*/
int32_t queue_push(struct queue_ctx *pctx, struct message_ctx *pval);
/*
* \brief          ����һ����
* \return         NULL ������
* \return         ERR_OK ������
*/
int32_t queue_pop(struct queue_ctx *pctx, struct message_ctx *pval);
int32_t queue_size(struct queue_ctx *pctx);
int32_t queue_cap(struct queue_ctx *pctx);

#endif//QUEUE_H_
