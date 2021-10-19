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
        PRINTF("ioctlsocket(%d, FIONREAD, nread) failed. %s", (int32_t)fd, ERRORSTR(ERRNO));
        return ERR_FAILED;
    }

    return (int32_t)ulread;
#else
    int32_t iread = 0;
    if (ioctl(fd, FIONREAD, &iread) < ERR_OK)
    {
        PRINTF("ioctl(%d, FIONREAD, nread) failed. %s", fd, ERRORSTR(ERRNO));
        return ERR_FAILED;
    }

    return iread;
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
* \brief          һ���໥���ӵ�socket
* \param sock     SOCKET
* \return         ERR_OK �ɹ�
*/
int32_t sockpair(SOCKET sock[2]);

#endif
