#ifndef QUEUE_H_
#define QUEUE_H_

#include "structs.h"

struct queue_ctx
{
    int32_t size;
    int32_t next;
    int32_t capacity;
    int32_t initcap;
    volatile atomic_t flags;
    struct message_ctx *msg;
};
/*
* \brief          ��ʼ��
*/
static inline void queue_init(struct queue_ctx *pctx, const int32_t icapacity)
{
    ASSERTAB((icapacity > 0 && icapacity <= INT_MAX), "capacity too large");
    pctx->size = 0;
    pctx->next = 0;
    pctx->flags = 0;
    pctx->capacity = ROUND_UP(icapacity, 2);
    pctx->initcap = pctx->capacity;
    pctx->msg = (struct message_ctx *)MALLOC(sizeof(struct message_ctx) * pctx->capacity);
    ASSERTAB(NULL != pctx->msg, ERRSTR_MEMORY);
};
/*
* \brief          �ͷ�
*/
static inline void queue_free(struct queue_ctx *pctx)
{
    FREE(pctx->msg);
};
/*
* \brief          ����
*/
static inline void queue_expand(struct queue_ctx *pctx)
{
    if (pctx->size < pctx->capacity)
    {
        return;
    }
    //����
    int32_t inewcap = pctx->capacity * 2;
    ASSERTAB((inewcap > 0 && inewcap <= INT_MAX), "capacity too large");
    struct message_ctx *pnew = (struct message_ctx *)MALLOC(sizeof(struct message_ctx) * inewcap);
    ASSERTAB(NULL != pnew, ERRSTR_MEMORY);

    for (int32_t i = 0; i < pctx->capacity; i++)
    {
        pnew[i] = pctx->msg[(pctx->next + i) % pctx->capacity];
    }
    FREE(pctx->msg);
    pctx->next = 0;
    pctx->msg = pnew;
    pctx->capacity = inewcap;
}
/*
* \brief          �������
* \param pval     ��Ҫ��ӵ�����
* \return         ERR_OK �ɹ�
*/
static inline int32_t queue_push(struct queue_ctx *pctx, struct message_ctx *pval)
{
    if (pctx->size >= pctx->capacity)
    {
        return ERR_FAILED;
    }

    int32_t ipos = pctx->next + pctx->size;
    if (ipos >= pctx->capacity)
    {
        ipos -= pctx->capacity;
    }
    pctx->msg[ipos] = *pval;
    pctx->size++;

    return ERR_OK;
};
/*
* \brief          ����һ����
* \return         NULL ������
* \return         ERR_OK ������
*/
static inline int32_t queue_pop(struct queue_ctx *pctx, struct message_ctx *pval)
{
    if (0 == pctx->size)
    {
        return ERR_FAILED;
    }
    
    *pval = pctx->msg[pctx->next];
    pctx->next++;
    pctx->size--;
    if (pctx->next >= pctx->capacity)
    {
        pctx->next -= pctx->capacity;
    }

    return ERR_OK;
};
static inline int32_t queue_size(struct queue_ctx *pctx)
{
    return pctx->size;
};
static inline int32_t queue_cap(struct queue_ctx *pctx)
{
    return pctx->capacity;
};

#endif//QUEUE_H_
