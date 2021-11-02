#ifndef BUFFER_H_
#define BUFFER_H_

#include "mutex.h"

#if defined(OS_WIN)
#define IOV_TYPE WSABUF
#define IOV_PTR_FIELD buf
#define IOV_LEN_FIELD len
#define IOV_LEN_TYPE ULONG
#else
//struct iovec {
//    void *iov_base;
//    size_t iov_len;
//};
#define IOV_TYPE struct iovec
#define IOV_PTR_FIELD iov_base
#define IOV_LEN_FIELD iov_len
#define IOV_LEN_TYPE size_t
#endif
typedef struct buffer_ctx
{
    struct buffernode_ctx *head;
    struct buffernode_ctx *tail;
    struct buffernode_ctx **tail_with_data;
    int32_t freeze_read;
    int32_t freeze_write;
    size_t total_len;//�����ܳ���
    mutex_ctx mutex;
}buffer_ctx;
typedef struct piece_node_ctx
{
    struct piece_node_ctx *next;
    size_t size;
    void *data;
}piece_node_ctx;
typedef struct piece_iov_ctx
{
    struct piece_node_ctx *head;
    struct piece_node_ctx *tail;
    void(*free_data)(void *);
}piece_iov_ctx;
/*
* \brief          ��ʼ��
*/
void buffer_init(struct buffer_ctx *pctx);
/*
* \brief          �ͷ�
*/
void buffer_free(struct buffer_ctx *pctx);
//��
static inline void buffer_lock(struct buffer_ctx *pctx)
{
    mutex_lock(&pctx->mutex);
};
static inline void buffer_unlock(struct buffer_ctx *pctx)
{
    mutex_unlock(&pctx->mutex);
};
/*
* \brief          �������
* \param pdata    ����
* \param uilen    ���ݳ���
* \return         ERR_OK �ɹ�
* \return         ERR_FAILED ʧ��
*/
int32_t _buffer_append(struct buffer_ctx *pctx, void *pdata, const size_t uilen);
static inline int32_t buffer_append(struct buffer_ctx *pctx, void *pdata, const size_t uilen)
{
    buffer_lock(pctx);
    int32_t irtn = _buffer_append(pctx, pdata, uilen);
    buffer_unlock(pctx);
    return irtn;
};
/*
* \brief          �������
* \param pfmt     ��ʽ
* \param ...      ���
* \return         ERR_OK �ɹ�
* \return         ERR_FAILED ʧ��
*/
int32_t buffer_appendv(struct buffer_ctx *pctx, const char *pfmt, ...);
/*
* \brief          �������ݣ���ɾ��
* \param pout     ��������ָ��
* \param uilen    ��������
* \return         ERR_FAILED ʧ��
* \return         ʵ�ʿ�����
*/
int32_t buffer_copyout(struct buffer_ctx *pctx, void *pout, size_t uilen);
/*
* \brief          ɾ������
* \param uilen    ɾ������
* \return         ERR_FAILED ʧ��
* \return         ʵ��ɾ����
*/
int32_t _buffer_drain(struct buffer_ctx *pctx, size_t uilen);
int32_t buffer_drain(struct buffer_ctx *pctx, size_t uilen);
/*
* \brief          ������ɾ������
* \param pout     ��������ָ��
* \param uilen    ��������
* \return         ERR_FAILED ʧ��
* \return         ʵ�ʿ���ɾ������
*/
int32_t buffer_remove(struct buffer_ctx *pctx, void *pout, size_t uilen);
/*
* \brief          ���������ֽڱȽ�
* \param uistart  ��ʼ������λ��
* \param pwhat    Ҫ����������
* \param uiwlens  pwhat����
* \return         ERR_FAILED δ�ҵ� ���� ͷ��������
* \return         ��һ�γ��ֵ�λ��
*/
int32_t buffer_search(struct buffer_ctx *pctx, const size_t uistart, void *pwhat, size_t uiwlens);
/*
* \brief          ���ݳ���
* \return         ���ݳ���
*/
static inline size_t buffer_size(struct buffer_ctx *pctx)
{
    buffer_lock(pctx);
    size_t uisize = pctx->total_len;
    buffer_unlock(pctx);
    return uisize;
}
/*
* \brief          ��չһ���ڵ㣬�����ڴ�
* \param uilen    ��չ����
* \return         ��չ�˵Ľڵ�
*/
struct buffernode_ctx *_buffer_expand_single(struct buffer_ctx *pctx, const size_t uilens);
/*
* \brief          ��չ�ڵ㣬�������ڴ�,������ݵ� piov��ʹ��_buffer_commit_iov�ύ���β���
* \param uilen    ��չ����
* \param piov     ��չ������ڴ洢��iov���� ����Ϊuicnt��
* \param uicnt    ���ʹ�ö��ٸ��ڵ�
* \return         ʵ����չ�Ľڵ�����С�ڵ���uicnt
*/
uint32_t _buffer_expand_iov(struct buffer_ctx *pctx, const size_t uilens,
    IOV_TYPE *piov, const uint32_t uicnt);
/*
* \brief          �ύ��������ݵ�iov����iov��_buffer_expand_iov��չ��
* \param uilens    ���ݳ���
* \param piov     iov����
* \param uicnt    piov����
* \return         ����˶�������
*/
size_t _buffer_commit_iov(struct buffer_ctx *pctx, size_t uilens ,IOV_TYPE *piov, const uint32_t uicnt);
/*
* \brief             ����buffer�е����ݲ�װ���piov
* \param uiatmost    ��Ҫװ������ݳ���
* \param piov        iov����
* \param uicnt       iov���鳤��
* \return            �����ݵ�piov����
*/
uint32_t _buffer_get_iov(struct buffer_ctx *pctx, size_t uiatmost,
    IOV_TYPE *piov, const uint32_t uicnt);

static inline uint32_t buffer_write_iov_application(struct buffer_ctx *pbuf, const size_t uisize,
    IOV_TYPE *piov, const uint32_t uiovcnt)
{
    buffer_lock(pbuf);
    ASSERTAB(0 == pbuf->freeze_write, "buffer tail already freezed.");
    pbuf->freeze_write = 1;
    uint32_t uicoun = _buffer_expand_iov(pbuf, uisize, piov, uiovcnt);
    buffer_unlock(pbuf);
    return uicoun;
};
static inline void buffer_write_iov_commit(struct buffer_ctx *pbuf, size_t ilens,
    IOV_TYPE *piov, const uint32_t uiovcnt)
{
    buffer_lock(pbuf);
    ASSERTAB(0 != pbuf->freeze_write, "buffer tail already unfreezed.");
    size_t uisize = _buffer_commit_iov(pbuf, ilens, piov, uiovcnt);
    ASSERTAB(uisize == ilens, "_buffer_commit_iov lens error.");
    pbuf->freeze_write = 0;
    buffer_unlock(pbuf);
};
static inline uint32_t buffer_read_iov_application(struct buffer_ctx *pbuf, size_t uisize,
    IOV_TYPE *piov, const uint32_t uiovcnt)
{
    buffer_lock(pbuf);
    ASSERTAB(0 == pbuf->freeze_read, "buffer head already freezed");
    uint32_t uicnt = _buffer_get_iov(pbuf, uisize, piov, uiovcnt);
    if (uicnt > 0)
    {
        pbuf->freeze_read = 1;
    }
    buffer_unlock(pbuf);
    return uicnt;
};
static inline void buffer_read_iov_commit(struct buffer_ctx *pbuf, size_t uisize)
{
    buffer_lock(pbuf);
    ASSERTAB(1 == pbuf->freeze_read, "buffer head already unfreezed.");
    if (uisize > 0)
    {
        ASSERTAB(uisize == _buffer_drain(pbuf, uisize), "drain size not equ commit size.");
    }
    pbuf->freeze_read = 0;
    buffer_unlock(pbuf);
};
static inline size_t _buffer_iov_size(IOV_TYPE *piov, const uint32_t uiovcnt)
{
    size_t isize = 0;
    for (uint32_t i = 0; i < uiovcnt; i++)
    {
        isize += (size_t)piov[i].IOV_LEN_FIELD;
    }
    return isize;
};
static inline void buffer_piece_node_free(struct piece_iov_ctx *ppiece)
{
    struct piece_node_ctx *pnext, *pnode = ppiece->head;
    while (NULL != pnode)
    {
        pnext = pnode->next;
        if (NULL != ppiece->free_data)
        {
            ppiece->free_data(pnode->data);
        }
        SAFE_FREE(pnode);
        pnode = pnext;
    }
};
static inline int32_t buffer_piece_inser(struct buffer_ctx *pbuf, struct piece_iov_ctx *ppiece,
    void *pnodedata, void *pdata, const size_t uilens)
{
    struct piece_node_ctx *pnode = MALLOC(sizeof(struct piece_node_ctx));
    if (NULL == pnode)
    {
        PRINTF("%s", ERRSTR_MEMORY);
        return ERR_FAILED;
    }
    pnode->data = pnodedata;
    pnode->size = uilens;
    pnode->next = NULL;

    buffer_lock(pbuf);
    if (NULL == ppiece->head)
    {
        ppiece->head = ppiece->tail = pnode;
    }
    else
    {
        ppiece->tail->next = pnode;
        ppiece->tail = pnode;
    }
    ASSERTAB(ERR_OK == _buffer_append(pbuf, pdata, uilens), "buffer_append error.");
    buffer_unlock(pbuf);

    return ERR_OK;
};
static inline uint32_t buffer_piece_read_iov_application(struct buffer_ctx *pbuf, struct piece_iov_ctx *ppiece, 
    IOV_TYPE *piov, const uint32_t uiovcnt, void **pdata)
{
    uint32_t uicnt = 0;
    buffer_lock(pbuf);
    ASSERTAB(0 == pbuf->freeze_read, "buffer head already freezed");
    if (NULL != ppiece->head)
    {
        *pdata = ppiece->head->data;
        uicnt = _buffer_get_iov(pbuf, ppiece->head->size, piov, uiovcnt);
        if (uicnt > 0)
        {
            ASSERTAB(ppiece->head->size == _buffer_iov_size(piov, uicnt),
                "the data size not equ node size.");
            pbuf->freeze_read = 1;
        }
    }
    buffer_unlock(pbuf);
    return uicnt;
};
static inline void buffer_piece_read_iov_commit(struct buffer_ctx *pbuf, struct piece_iov_ctx *ppiece, size_t uilens)
{
    buffer_lock(pbuf);
    ASSERTAB(1 == pbuf->freeze_read, "buffer head already unfreezed.");
    if (uilens > 0)
    {
        struct piece_node_ctx *pnode = ppiece->head;
        ASSERTAB(NULL != pnode && uilens == pnode->size, "buffer head is NULL or commit size not equ node size.");
        ASSERTAB(uilens == _buffer_drain(pbuf, uilens), "drain size not equ commit size.");
        if (NULL == pnode->next)
        {
            ppiece->head = ppiece->tail = NULL;
        }
        else
        {
            ppiece->head = pnode->next;
        }
        if (NULL != ppiece->free_data)
        {
            ppiece->free_data(pnode->data);
        }
        SAFE_FREE(pnode);
    }
    pbuf->freeze_read = 0;
    buffer_unlock(pbuf);
};

#endif//BUFFER_H_
