#ifndef EVENT_H_
#define EVENT_H_

#include "macro.h"
#include "timer.h"
#include "thread.h"
#include "wot.h"
#include "iocp/iocp.h"
#include "epoll/epoll.h"
#include "evport/evport.h"
#include "kqueue/kqueue.h"

typedef struct server_ctx
{
    uint64_t accuracy;
    struct netio_ctx *netio;
    volatile atomic_t stop;//ֹͣ��־
    struct wot_ctx wot;
    struct timer_ctx timer;
    struct chan_ctx chan;
    struct thread_ctx thread;
}server_ctx;
/*
* \brief               ��ʼ��
* \param ulaccuracy    ʱ�侫��
*/
void server_init(struct server_ctx *pctx, uint64_t ulaccuracy);
/*
* \brief               ֹͣ���ͷ�
*/
void server_free(struct server_ctx *pctx);
/*
* \brief               ��ʼ����
*/
void server_run(struct server_ctx *pctx);
/*
* \brief               ��ǰʱ��tick
*/
static inline u_long server_tick(struct server_ctx *pctx)
{
    return (u_long)(timer_nanosec(&pctx->timer) / pctx->accuracy);
}
/*
* \brief               ���һ��ʱ�¼�
* \param  pchan        ���ճ�ʱ��Ϣ, ��Ҫ�ֶ��ͷ� ev_ctx��twnode_ctx��
* \param  uitick       ��ʱʱ��  ���ٸ�tick
* \param  pdata        �û�����
* \return              ERR_OK �ɹ�
*/
static inline int32_t server_timeout(struct server_ctx *pctx,
    struct chan_ctx *pchan, const uint32_t uitick, const void *pdata)
{
    return wot_add(&pctx->wot, pchan, server_tick(pctx), uitick, pdata);
};
/*
* \brief          �½�һ����
* \param pchan    chan ����EV_ACCEPT��Ϣ, ��Ҫ�ֶ��ͷ�ev_ctx
* \param ichannum pchan ����
* \param phost    ip
* \param usport   port
* \return         sock_ctx
*/
static inline struct sock_ctx *server_listen(struct server_ctx *pctx, struct chan_ctx *pchan,
    size_t ichannum, const char *phost, const uint16_t usport)
{
    return netio_listen(pctx->netio, pchan, ichannum, phost, usport);
};
/*
* \brief          �½�һ����
* \param pchan    chan ����EV_CONNECT��Ϣ, ��Ҫ�ֶ��ͷ�ev_ctx
* \param phost    ip
* \param usport   port
* \return         sock_ctx
*/
static inline struct sock_ctx *server_connect(struct server_ctx *pctx, struct chan_ctx *pchan,
    const char *phost, const uint16_t usport)
{
    return netio_connect(pctx->netio, pchan, phost, usport);
};
/*
* \brief          ������SOCKET�ӽ�ȥ
* \param fd       socket���
* \return         sock_ctx
*/
static inline struct sock_ctx *server_addsock(struct server_ctx *pctx, SOCKET fd)
{
    return netio_addsock(pctx->netio, fd);
};
/*
* \brief                 �ɶ�д���ڵ���server_addsock�ɹ����յ� EV_ACCEPT��EV_CONNECT��Ϣ�����һ��
*                        ��Ҫ�ֶ��ͷ�ev_ctx
* \param pchan           chan ����EV_RECV��EV_SEND��Ϣ
* \param postsendev      �Ƿ�Ͷ��EV_SEND��Ϣ, 0��Ͷ��
* \return                ERR_OK �ɹ�
*/
static inline int32_t server_enable_rw(struct sock_ctx *psock, struct chan_ctx *pchan, const uint8_t postsendev)
{
    return netio_enable_rw(psock, pchan, postsendev);
};
/*
* \brief          �ر�socket  �յ�EV_CLOSE���ͷ���Դ
* \param fd       socket���
* \return         sock_ctx
*/
static inline void server_close(struct sock_ctx *psock)
{
    netio_close(psock);
};
/*
* \brief          ������Ϣ
* \return         ERR_OK �ɹ�
*/
static inline int32_t server_send(struct sock_ctx *psock, void *pdata, size_t ilens)
{
    return netio_send(psock, pdata, ilens);
};
/*
* \brief          udp������Ϣ
* \return         ERR_OK �ɹ�
*/
static inline int32_t server_sendto(struct sock_ctx *psock, const char *phost, uint16_t usport,
    IOV_TYPE *wsabuf, size_t uicount)
{
    return netio_sendto(psock, phost, usport, wsabuf, uicount);
};

#endif//EVENT_H_
