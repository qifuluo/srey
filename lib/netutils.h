#ifndef NETUTILS_H_
#define NETUTILS_H_

#include "macro.h"

/*
* \brief          ��ȡsocket�ɶ�����
* \param fd       socket���
* \return         ERR_FAILED ʧ��
* \return         ����
*/
static inline int32_t socknread(SOCKET fd)
{
#if defined(OS_WIN)
    u_long ulread = 0;
    if (ioctlsocket(fd, FIONREAD, &ulread) < ERR_OK)
    {
        return ERR_FAILED;
    }

    return (int32_t)ulread;
#else
    int32_t iread = 0;
    if (ioctl(fd, FIONREAD, &iread) < ERR_OK)
    {
        return ERR_FAILED;
    }

    return iread;
#endif
};
/*
* \brief          ��ȡsa_family
* \param fd       SOCKET
* \return         ERR_FAILED ʧ��
* \return         SOCK_STREAM  SOCK_DGRAM
*/
static inline int32_t socktype(SOCKET fd)
{
    int32_t itype = 0;
    int32_t ilen = (int32_t)sizeof(itype);
    if (getsockopt(fd, SOL_SOCKET, SO_TYPE, (char *)&itype, (socklen_t*)&ilen) < ERR_OK)
    {
        PRINTF("getsockopt(%d, ...) failed. %s", (int32_t)fd, ERRORSTR(ERRNO));
        return ERR_FAILED;
    }

    return itype;
};
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
#ifdef SO_DOMAIN
    int32_t ifamily = 0;
    int32_t ilens = (int32_t)sizeof(ifamily);
    if (getsockopt(fd, SOL_SOCKET, SO_DOMAIN, &ifamily, (socklen_t*)&ilens) < 0)
    {
        PRINTF("getsockopt(%d, SOL_SOCKET, SO_DOMAIN, ...) failed. %s", (int32_t)fd, ERRORSTR(ERRNO));
        return ERR_FAILED;
    }
    return ifamily;
#endif
    return AF_INET;
#endif
};
/*
* \brief          ����socket TCP_NODELAY
* \param fd       SOCKET
*/
void socknodelay(SOCKET fd);
/*
* \brief          ����socket ������
* \param fd       SOCKET
*/
void socknbio(SOCKET fd);
/*
* \brief          ���õ�ַ����
* \param fd       SOCKET
*/
void sockraddr(SOCKET fd);
/*
* \brief          �Ƿ�֧�ֶ˿�����
* \return         ERR_OK֧��
*/
int32_t checkrport();
/*
* \brief          ���ö˿�����
* \param fd       ������SOCKET
*/
void sockrport(SOCKET lsfd);
/*
* \brief            ����socket SO_KEEPALIVE
* \param fd         SOCKET
* \param idelay     �೤ʱ��û�б��Ŀ�ʼ����keepalive ��   С�ڵ���0������ʱ������ʹ��ϵͳĬ�ϵ�
* \param iintvl     ����keepalive����ʱ���� ��
*/
void sockkpa(SOCKET fd, const int32_t idelay, const int32_t iintvl);
/*
* \brief          ����SO_LINGER
* \param fd       SOCKET
*/
void closereset(SOCKET fd);
/*
* \brief          һ���໥���ӵ�socket
* \param sock     SOCKET
* \return         ERR_OK �ɹ�
*/
int32_t sockpair(SOCKET sock[2]);

#endif
