#include "netaddr.h"

static inline void _clear(struct netaddr_ctx *pctx)
{
    ZERO(&pctx->ipv4, sizeof(pctx->ipv4));
    ZERO(&pctx->ipv6, sizeof(pctx->ipv6));
    pctx->ipv4.sin_family = AF_INET;
    pctx->ipv6.sin6_family = AF_INET6;
}
static inline int32_t _checkipv4(const char *phost)
{
    for (int32_t i = 0; i < (int32_t)strlen(phost); i++)
    {
        if (phost[i] == '.')
        {
            return ERR_OK;
        }
        if (phost[i] == ':')
        {
            return ERR_FAILED;
        }
        if (phost[i] < '0' || phost[i] > '9')
        {
            return ERR_FAILED;
        }
    }

    return ERR_FAILED;
}
int32_t netaddr_sethost(struct netaddr_ctx *pctx, const char *phost, const uint16_t usport)
{
    _clear(pctx);
    if (ERR_OK == _checkipv4(phost))
    {
        pctx->type = AF_INET;
        int32_t irtn = inet_pton(AF_INET, phost, &pctx->ipv4.sin_addr.s_addr);
        if (irtn < ERR_OK)
        {
            return irtn;
        }
        pctx->ipv4.sin_port = htons(usport);
    }
    else
    {
        pctx->type = AF_INET6;
        int32_t irtn = inet_pton(AF_INET6, phost, &pctx->ipv6.sin6_addr.s6_addr);
        if (irtn < ERR_OK)
        {
            return irtn;
        }
        pctx->ipv6.sin6_port = htons(usport);
    }
    return ERR_OK;
}
void netaddr_setaddr(struct netaddr_ctx *pctx, const struct sockaddr *paddr)
{
    _clear(pctx);
    if (AF_INET == paddr->sa_family)
    {
        pctx->type = AF_INET;
        memcpy(&pctx->ipv4, paddr, sizeof(pctx->ipv4));
    }
    else
    {
        pctx->type = AF_INET6;
        memcpy(&pctx->ipv6, paddr, sizeof(pctx->ipv6));
    }
}
int32_t netaddr_remoteaddr(struct netaddr_ctx *pctx, SOCKET fd, const int32_t ifamily)
{
    if (INVALID_SOCK == fd)
    {
        return ERR_FAILED;
    }
    if (AF_INET == ifamily)
    {
        struct sockaddr_in addr = { 0 };
        socklen_t iaddrlens = (socklen_t)sizeof(struct sockaddr_in);
        if (ERR_OK != getpeername(fd, (struct sockaddr *)&addr, &iaddrlens))
        {
            return ERRNO;
        }
        netaddr_setaddr(pctx, (struct sockaddr *)&addr);
    }
    else
    {
        struct sockaddr_in6 addr = { 0 };
        socklen_t iaddrlens = (socklen_t)sizeof(struct sockaddr_in6);
        if (ERR_OK != getpeername(fd, (struct sockaddr *)&addr, &iaddrlens))
        {
            return ERRNO;
        }
        netaddr_setaddr(pctx, (struct sockaddr *)&addr);
    }
    
    return ERR_OK;
}
int32_t netaddr_localaddr(struct netaddr_ctx *pctx, SOCKET fd, const int32_t ifamily)
{
    if (INVALID_SOCK == fd)
    {
        return ERR_FAILED;
    }
    if (AF_INET == ifamily)
    {
        struct sockaddr_in addr = { 0 };
        socklen_t iaddrlens = (socklen_t)sizeof(struct sockaddr_in);
        if (ERR_OK != getsockname(fd, (struct sockaddr *)&addr, &iaddrlens))
        {
            return ERRNO;
        }
        netaddr_setaddr(pctx, (struct sockaddr *)&addr); 
    }
    else
    {
        struct sockaddr_in6 addr = { 0 };
        socklen_t iaddrlens = (socklen_t)sizeof(struct sockaddr_in6);
        if (ERR_OK != getsockname(fd, (struct sockaddr *)&addr, &iaddrlens))
        {
            return ERRNO;
        }
        netaddr_setaddr(pctx, (struct sockaddr *)&addr);
    }

    return ERR_OK;
}
