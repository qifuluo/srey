#ifndef EVTYPE_H_
#define EVTYPE_H_

#include "netaddr.h"

#define EV_TIME     0x01
#define EV_ACCEPT   0x02
#define EV_CONNECT  0x03
#define EV_CLOSE    0x04
#define EV_RECV     0x05
#define EV_SEND     0x06
typedef struct ev_ctx
{
    int32_t evtype;     //����  
    int32_t code;       //ERR_OK �ɹ�  EV_RECV  EV_SEND �ĕr���ʾ���ݳ���
    void *data;         //�û�����
}ev_ctx;
static inline struct ev_ctx *ev_new_accept(struct sock_ctx *psock)
{
    struct ev_ctx *pev = (struct ev_ctx *)MALLOC(sizeof(struct ev_ctx));
    ASSERTAB(NULL != pev, ERRSTR_MEMORY);
    pev->code = ERR_OK;
    pev->data = psock;
    pev->evtype = EV_ACCEPT;

    return pev;
}
static inline struct ev_ctx *ev_new_connect(struct sock_ctx *psock, const int32_t icode)
{
    struct ev_ctx *pev = (struct ev_ctx *)MALLOC(sizeof(struct ev_ctx));
    ASSERTAB(NULL != pev, ERRSTR_MEMORY);
    pev->code = icode;
    pev->data = psock;
    pev->evtype = EV_CONNECT;

    return pev;
}
static inline struct ev_ctx *ev_new_close(struct sock_ctx *psock)
{
    struct ev_ctx *pev = (struct ev_ctx *)MALLOC(sizeof(struct ev_ctx));
    ASSERTAB(NULL != pev, ERRSTR_MEMORY);
    pev->code = ERR_OK;
    pev->data = psock;
    pev->evtype = EV_CLOSE;

    return pev;
}
static inline struct ev_ctx *ev_new_recv(struct sock_ctx *psock, const size_t ilens)
{
    struct ev_ctx *pev = (struct ev_ctx *)MALLOC(sizeof(struct ev_ctx));
    ASSERTAB(NULL != pev, ERRSTR_MEMORY);
    pev->code = (int32_t)ilens;
    pev->data = psock;
    pev->evtype = EV_RECV;

    return pev;
}
static inline struct ev_ctx *ev_new_send(struct sock_ctx *psock, const size_t ilens)
{
    struct ev_ctx *pev = (struct ev_ctx *)MALLOC(sizeof(struct ev_ctx));
    ASSERTAB(NULL != pev, ERRSTR_MEMORY);
    pev->code = (int32_t)ilens;
    pev->data = psock;
    pev->evtype = EV_SEND;

    return pev;
}

#endif//EVTYPE_H_
