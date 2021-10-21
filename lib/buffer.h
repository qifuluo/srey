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
    int32_t freeze_head;
    int32_t freeze_tail;
    size_t total_len;//�����ܳ���
    mutex_ctx mutex;
}buffer_ctx;

/*
* \brief          ��ʼ��
*/
void buffer_init(struct buffer_ctx *pctx);
/*
* \brief          �ͷ�
*/
void buffer_free(struct buffer_ctx *pctx);
/*
* \brief          �������
* \param pdata    ����
* \param uilen    ���ݳ���
* \return         ERR_OK �ɹ�
* \return         ERR_FAILED ʧ��
*/
int32_t buffer_append(struct buffer_ctx *pctx, void *pdata, const size_t uilen);
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
int32_t buffer_search(struct buffer_ctx *pctx, size_t uistart, void *pwhat, size_t uiwlens);
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
* \param piov     ��չ������ڴ洢��iov���� ����Ϊuicount��
* \param uicount  ���ʹ�ö��ٸ��ڵ�
* \return         ʵ����չ�Ľڵ�����С�ڵ���uicount
*/
size_t _buffer_expand_iov(struct buffer_ctx *pctx, const size_t uilens, 
    IOV_TYPE *piov, const size_t uicount);
/*
* \brief          �ύ��������ݵ�iov����iov��_buffer_expand_iov��չ��
* \param piov     iov����
* \param uicount  piov����
* \return         ����˶�������
*/
size_t _buffer_commit_iov(struct buffer_ctx *pctx, IOV_TYPE *piov, const size_t uicount);
/*
* \brief             ����buffer�е����ݲ�װ���piov
* \param uihowmuch   ��Ҫװ������ݳ���
* \param piov        iov����
* \param uiovsize    iov���鳤��
* \param prealsize   ʵ��װ������ݳ���
* \return            �����ݵ�piov����
*/
size_t _buffer_get_iov(struct buffer_ctx *pctx, size_t uihowmuch, 
    IOV_TYPE *piov, size_t uiovsize, size_t *prealsize);

#endif//BUFFER_H_
