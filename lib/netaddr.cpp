#include "netaddr.h"
#include "errcode.h"

SREY_NS_BEGIN

bool cnetaddr::setaddr(const char *ip, const uint16_t &port)
{
    struct addrinfo staddr = { 0 };
    struct addrinfo *paddr = NULL;

    staddr.ai_flags = AI_PASSIVE;
    staddr.ai_family = AF_INET;

    int32_t irtn = getaddrinfo(ip, NULL, &staddr, &paddr);
    if (ERR_OK != irtn)
    {
        PRINTF("getaddrinfo(%s) error, message %s.", ip, gai_strerror(irtn));
        if (NULL != paddr)
        {
            freeaddrinfo(paddr);
        }

        return false;
    }
    memcpy(&ipv4, paddr->ai_addr, paddr->ai_addrlen);
    ipv4.sin_port = htons(port);
    freeaddrinfo(paddr);

    return true;
}
void cnetaddr::setaddr(const struct sockaddr *paddr)
{
    memcpy(&ipv4, paddr, sizeof(ipv4));
}
bool cnetaddr::setremaddr(const SOCKET &fd)
{
    sockaddr staddr;
    socklen_t ilens = (socklen_t)sizeof(staddr);
    ZERO(&staddr, ilens);

    if (ERR_OK != getpeername(fd, &staddr, &ilens))
    {
        return false;
    }
    setaddr(&staddr);

    return true;
}
bool cnetaddr::setlocaddr(const SOCKET &fd)
{
    sockaddr staddr;
    socklen_t ilens = (socklen_t)sizeof(staddr);
    ZERO(&staddr, ilens);

    if (ERR_OK != getsockname(fd, &staddr, &ilens))
    {
        return false;
    }
    setaddr(&staddr);

    return true;
}
sockaddr *cnetaddr::getaddr(void)
{
    return (sockaddr*)&ipv4;
}
size_t cnetaddr::getsize(void)
{
    return sizeof(ipv4);
}
std::string cnetaddr::getip(void)
{
    char atmp[128] = { 0 };
    int32_t ilens = sizeof(atmp);
    if (ERR_OK != getnameinfo((const sockaddr*)&ipv4, (socklen_t)sizeof(ipv4), atmp, ilens, NULL, 0, NI_NUMERICHOST))
    {
        return "";
    }

    return atmp;
}
uint16_t cnetaddr::getport()
{
    return ntohs(ipv4.sin_port);
}

SREY_NS_END
