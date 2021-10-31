#ifndef EVENT_H_
#define EVENT_H_

#include "timer.h"
#include "thread.h"
#include "wot.h"
#include "iocp/iocp.h"
#include "epoll/epoll.h"
#include "evport/evport.h"
#include "kqueue/kqueue.h"

typedef struct event_ctx
{
    u_long accuracy;//��ʱ������
    volatile atomic_t stop;//ֹͣ��־
    struct wot_ctx wot;//ʱ����
    struct timer_ctx timer;//��ʱ��
    struct chan_ctx chfree;//�ӳ��ͷ�
    struct thread_ctx thwot;//��ʱ�߳�
    struct thread_ctx thfree;
    struct netev_ctx netev;//����
}event_ctx;
/*
* \brief             ��ʼ��
* \param ulaccuracy  ʱ�侫��
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
* \param pchan     chan ����EV_ACCEPT��Ϣ, ��Ҫ�ֶ��ͷ�ev_ctx(ev_sock_ctx)
* \param uchancnt  pchan ����
* \param phost     ip
* \param usport    port
* \return          INVALID_SOCK ʧ��
*/
static inline SOCKET event_listener(struct event_ctx *pctx, struct chan_ctx *pchan,
    const uint16_t uchancnt, const char *phost, const uint16_t usport)
{
    return netev_listener(&pctx->netev, pchan, uchancnt, phost, usport);
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
    return netev_connecter(&pctx->netev, pchan, phost, usport);
};
/*
* \brief            ������SOCKET�ӽ�ȥ,ʧ�ܹر�fd���
* \param fd         socket���
* \return           ERR_OK �ɹ�
*/
static inline int32_t event_addsock(struct event_ctx *pctx, SOCKET fd)
{
    return netev_addsock(&pctx->netev, fd);
};
/*
* \brief              ��ʼ�������ݡ��ڵ���event_addsock�ɹ����յ� EV_ACCEPT��EV_CONNECT��Ϣ�����һ��
*                     ʧ�ܹر�fd���
* \param pchan        ����EV_RECV��EV_SEND��EV_CLOSE��Ϣ
* \param upostsendev  0 ������EV_SEND
* \return             sock_ctx
*/
static inline struct sock_ctx *event_enable_rw(SOCKET fd, struct chan_ctx *pchan, const uint16_t upostsendev)
{
    return netev_enable_rw(fd, pchan, upostsendev);
};
/*
* \brief              ��ʼ�������ݡ��ڵ���event_addsock�ɹ����յ� EV_ACCEPT��EV_CONNECT��Ϣ�����һ��
*                     ʧ�ܹر�fd���
* \param pchan        ����EV_RECV��EV_SEND��EV_CLOSE��Ϣ
* \param upostsendev  0 ������EV_SEND
* \return             sender_ctx
*/
static inline void event_sock_free(struct event_ctx *pctx, struct sock_ctx *psockctx)
{
    if (ERR_OK == _sock_can_free(psockctx))
    {
        _sock_free(psockctx);
    }
    else
    {
        event_timeout(pctx, &pctx->chfree, 10, psockctx);
    }
}

#endif//EVENT_H_
