#ifndef QUEUE_H_
#define QUEUE_H_

#include "macro.h"

typedef struct queue_ctx
{
    int32_t size;
    int32_t next;
    int32_t capacity;
    void **data;
}queue_ctx;
/*
* \brief          ��ʼ��
*/
static inline void queue_init(struct queue_ctx *pctx, const int32_t icapacity)
{
    ASSERTAB((icapacity > 0 && icapacity <= INT_MAX / (int32_t)sizeof(void*)), "capacity too large");
    pctx->size = 0;
    pctx->next = 0;
    pctx->capacity = icapacity;
    pctx->data = (void **)MALLOC(sizeof(void*) * pctx->capacity);
    ASSERTAB(NULL != pctx->data, ERRSTR_MEMORY);
};
/*
* \brief          �ͷ�
*/
static inline void queue_free(struct queue_ctx *pctx)
{
    SAFE_FREE(pctx->data);
};
/*
* \brief          �������
* \param pval     ��Ҫ��ӵ�����
* \return         ERR_OK �ɹ�
*/
static inline int32_t queue_push(struct queue_ctx *pctx, void *pval)
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
    pctx->data[ipos] = pval;
    pctx->size++;

    return ERR_OK;
};
/*
* \brief          ����һ����
* \return         NULL ������
* \return         ����
*/
static inline void *queue_pop(struct queue_ctx *pctx)
{
    void *pval = NULL;

    if (pctx->size > 0)
    {
        pval = pctx->data[pctx->next];
        pctx->next++;
        pctx->size--;
        if (pctx->next >= pctx->capacity)
        {
            pctx->next -= pctx->capacity;
        }
    }

    return pval;
};
/*
* \brief          ��ȡ��һ������
* \return         NULL ������
* \return         ����
*/
static inline void *queue_peek(struct queue_ctx *pctx)
{
    return pctx->size > 0 ? pctx->data[pctx->next] : NULL;
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
