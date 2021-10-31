#ifndef NETADDR_H_
#define NETADDR_H_

#include "macro.h"

struct netaddr_ctx
{
    int32_t type;
    struct sockaddr_in	ipv4;
    struct sockaddr_in6 ipv6;
}netaddr_ctx;
/*
* \brief          ��ȡsin_family
* \param fd       SOCKET
* \return         ERR_FAILED ʧ��
*/
static inline int32_t sockaddrfamily(SOCKET fd)
{
#if defined(OS_WIN)
    WSAPROTOCOL_INFO info;
    int32_t ilens = (int32_t)sizeof(info);
    if (getsockopt(fd, SOL_SOCKET, SO_PROTOCOL_INFO, (char *)&info, &ilens) < ERR_OK)
    {
        PRINTF("getsockopt(%d, SOL_SOCKET, SO_PROTOCOL_INFO, ...) failed. %s", (int32_t)fd, ERRORSTR(ERRNO));
        return ERR_FAILED;
    }
    return info.iAddressFamily;
#else
    int32_t ifamily = 0;
    int32_t ilens = (int32_t)sizeof(ifamily);
    if (getsockopt(fd, SOL_SOCKET, SO_DOMAIN, (char*)&ifamily, &ilens) < 0)
    {
        PRINTF("getsockopt(%d, SOL_SOCKET, SO_DOMAIN, ...) failed. %s", (int32_t)fd, ERRORSTR(ERRNO));
        return ERR_FAILED;
    }
    return ifamily;
#endif
}
/*
* \brief          ���õ�ַ
* \param phost    ip
* \param usport   port
* \return         ERR_OK �ɹ� gai_strerror��ȡ������Ϣ
*/
int32_t netaddr_sethost(struct netaddr_ctx *pctx, const char *phost, const uint16_t usport);
/*
* \brief          ���õ�ַ
* \param phost    ip
* \param usport   port
* \return         ERR_OK �ɹ�
*/
void netaddr_setaddr(struct netaddr_ctx *pctx, const struct sockaddr *paddr);
/*
* \brief          ��ȡԶ�˵�ַ��Ϣ
* \param fd       SOCKET
* \return         ERR_OK �ɹ�
*/
int32_t netaddr_remoteaddr(struct netaddr_ctx *pctx, SOCKET fd);
/*
* \brief          ��ȡ���ص�ַ��Ϣ
* \param fd       SOCKET
* \return         ERR_OK �ɹ�
*/
int32_t netaddr_localaddr(struct netaddr_ctx *pctx, SOCKET fd);
/*
* \brief          ���ص�ַ
* \return         sockaddr *
*/
static inline struct sockaddr *netaddr_addr(struct netaddr_ctx *pctx)
{
    if (AF_INET == pctx->type)
    {
        return (struct sockaddr*)&pctx->ipv4;
    }
    else
    {
        return (struct sockaddr*)&pctx->ipv6;
    }
};
/*
* \brief          ��ַ����
* \return         ��ַ����
*/
static inline socklen_t netaddr_size(struct netaddr_ctx *pctx)
{
    if (AF_INET == pctx->type)
    {
        return (socklen_t)sizeof(pctx->ipv4);
    }
    else
    {
        return (socklen_t)sizeof(pctx->ipv6);
    }
};
/*
* \brief          ��ȡIP
* \param acip     ip
* \return         ERR_OK �ɹ�
*/
static inline int32_t netaddr_ip(struct netaddr_ctx *pctx, char acip[IP_LENS])
{
    ZERO(acip, IP_LENS);
    if (AF_INET == pctx->type)
    {
        if (NULL == inet_ntop(AF_INET, &pctx->ipv4.sin_addr, acip, IP_LENS))
        {
            PRINTF("inet_ntop failed, %s", ERRORSTR(ERRNO));
            return ERR_FAILED;
        }
    }
    else
    {
        if (NULL == inet_ntop(AF_INET6, &pctx->ipv6.sin6_addr, acip, IP_LENS))
        {
            PRINTF("inet_ntop failed, %s", ERRORSTR(ERRNO));
            return ERR_FAILED;
        }
    }

    return ERR_OK;
};
/*
* \brief          ��ȡ�˿�
* \return         �˿�
*/
static inline uint16_t netaddr_port(struct netaddr_ctx *pctx)
{
    if (AF_INET == pctx->type)
    {
        return ntohs(pctx->ipv4.sin_port);
    }
    else
    {
        return ntohs(pctx->ipv6.sin6_port);
    }
};
/*
* \return         AF_INET or AF_INET6;
*/
static inline uint16_t netaddr_family(struct netaddr_ctx *pctx)
{
    return pctx->type;
};

#endif//NETADDR_H_
