#ifndef EVENT_H_
#define EVENT_H_

#include "timer.h"
#include "thread.h"
#include "wot.h"
#include "netapi.h"

typedef struct event_ctx
{
    u_long accuracy;//��ʱ������
    int32_t stop;//ֹͣ��־
    struct wot_ctx wot;//ʱ����
    struct timer_ctx timer;//��ʱ��
    struct chan_ctx chfree;//�ӳ��ͷ�
    struct thread_ctx thwot;//��ʱ�߳�
    struct thread_ctx thfree;
    struct netev_ctx *netev;//����
}event_ctx;
/*
* \brief             ��ʼ��
* \param ulaccuracy  ʱ�侫�� 0 Ĭ�� 10����
*/
void event_init(struct event_ctx *pctx, const u_long ulaccuracy);
/*
* \brief             ֹͣ���ͷ�
*/
void event_free(struct event_ctx *pctx);
/*
* \brief             ��ʼ����
*/
void event_loop(struct event_ctx *pctx);
/*
* \brief             ��ǰtick
*/
static inline u_long event_tick(struct event_ctx *pctx)
{
    return (u_long)(timer_nanosec(&pctx->timer) / pctx->accuracy);
};
/*
* \brief           ���һ��ʱ�¼�
* \param  pchan    ���ճ�ʱ��Ϣ, ��Ҫ�ֶ��ͷ� ev_ctx��ev_time_ctx��
* \param  uitick   ��ʱʱ��  ���ٸ�tick
* \param  pdata    �û�����
* \return          ERR_OK �ɹ�
*/
static inline void event_timeout(struct event_ctx *pctx,
    struct chan_ctx *pchan, const uint32_t uitick, const void *pdata)
{
    wot_add(&pctx->wot, pchan, event_tick(pctx), uitick, pdata);
};
/*
* \brief           �½�һ����
* \param pchan     chan ����EV_ACCEPT��Ϣ, ���յ���socketͨ��event_addsock�ӽ�����,
 *                 ��Ҫ�ֶ��ͷ�ev_ctx(ev_sock_ctx)
* \param uichancnt pchan ����
* \param phost     ip
* \param usport    port
* \return          INVALID_SOCK ʧ��
*/
static inline SOCKET event_listener(struct event_ctx *pctx, struct chan_ctx *pchan,
    const uint32_t uichancnt, const char *phost, const uint16_t usport)
{
    return netev_listener(pctx->netev, pchan, uichancnt, phost, usport);
};
/*
* \brief            �½�һ����
* \param pchan      chan ����EV_CONNECT��Ϣ, ��Ҫ�ֶ��ͷ�ev_ctx(ev_sock_ctx)
* \param phost      ip
* \param usport     port
* \return           INVALID_SOCK ʧ��
*/
static inline SOCKET event_connecter(struct event_ctx *pctx, struct chan_ctx *pchan,
    const char *phost, const uint16_t usport)
{
    return netev_connecter(pctx->netev, pchan, phost, usport);
};
/*
* \brief            ������SOCKET�ӽ�ȥ,ʧ�ܹر�fd���, EV_ACCEPT ���Զ���socket
* \param fd         socket���
* \return           ERR_OK �ɹ�
*/
static inline int32_t event_addsock(struct event_ctx *pctx, SOCKET fd)
{
    return netev_addsock(pctx->netev, fd);
};
/*
* \brief              ��ʼ�������ݡ��ڵ���event_addsock�ɹ����յ�EV_CONNECT��Ϣ�����һ��
*                     ʧ�ܹر�fd���
* \param pchan        ����EV_RECV��EV_SEND��EV_CLOSE��Ϣ
* \param ipostsendev  0 ������EV_SEND
* \return             sock_ctx
*/
static inline struct sock_ctx *event_enablerw(struct event_ctx *pctx, SOCKET fd, struct chan_ctx *pchan, const int32_t ipostsendev)
{
    return netev_enable_rw(pctx->netev, fd, pchan, ipostsendev);
};
/*
* \brief              �ͷ�sock_ctx
* \param pctx         event_ctx
* \param psockctx     psockctx
*/
static inline void event_freesock(struct event_ctx *pctx, struct sock_ctx *psockctx)
{
    if (ERR_OK == _sock_can_free(psockctx))
    {
        _sock_free(psockctx);
    }
    else
    {
        event_timeout(pctx, &pctx->chfree, 5, psockctx);
    }
};
/*
* \brief              �ر�socket
* \param psockctx     sock_ctx
*/
static inline void event_closesock(struct sock_ctx *psockctx)
{
    sock_close(psockctx);
};
/*
* \brief              ���Ľ������ݵ�pchan
* \param psockctx     event_enable_rw ����ֵ
* \param pchan        chan_ctx
*/
static inline void event_changechan(struct sock_ctx *psockctx, struct chan_ctx *pchan)
{
    sock_change_chan(psockctx, pchan);
};
static inline struct buffer_ctx *event_recvbuf(struct sock_ctx *psockctx)
{
    return sock_recvbuf(psockctx);
};
static inline struct buffer_ctx *event_sendbuf(struct sock_ctx *psockctx)
{
    return sock_sendbuf(psockctx);
};
static inline SOCKET event_handle(struct sock_ctx *psockctx)
{
    return sock_handle(psockctx);
};
static inline int32_t event_send(struct sock_ctx *psockctx, void *pdata, const size_t uilens)
{
    return sock_send(psockctx, pdata, uilens);
};
static inline int32_t event_send_buf(struct sock_ctx *psockctx)
{
    return sock_send_buf(psockctx);
};

#endif//EVENT_H_
