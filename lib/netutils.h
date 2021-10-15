#ifndef NETUTILS_H_
#define NETUTILS_H_

#include "macro.h"

SREY_NS_BEGIN

/*
* \brief          ��ȡsocket�ɶ�����
* \param fd       socket���
* \return         ERR_FAILED ʧ��
* \return         ����
*/
inline int32_t socknread(const SOCKET &fd)
{
#ifdef OS_WIN
    u_long ulread = INIT_NUMBER;
    if (ioctlsocket(fd, FIONREAD, &ulread) < ERR_OK)
    {
        PRINTF("ioctlsocket(%d, FIONREAD, nread) failed. %s", (int32_t)fd, ERRORSTR(ERRNO));
        return ERR_FAILED;
    }

    return (int32_t)ulread;
#else
    int32_t iread = INIT_NUMBER;
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
void socknodelay(const SOCKET &fd);
/*
* \brief          ����socket ������
* \param fd       SOCKET
*/
void socknbio(const SOCKET &fd);
/*
* \brief          ���õ�ַ����
* \param fd       SOCKET
*/
void sockraddr(const SOCKET &fd);
/*
* \brief          �Ƿ�֧�ֶ˿�����
* \return         true֧��
*/
bool checkrport();
/*
* \brief          ���ö˿�����
* \param fd       ������SOCKET
*/
void sockrport(const SOCKET &lsfd);
/*
* \brief            ����socket SO_KEEPALIVE
* \param fd         SOCKET
* \param idelay     �೤ʱ��û�б��Ŀ�ʼ����keepalive ��   С�ڵ���0������ʱ������ʹ��ϵͳĬ�ϵ�
* \param iintvl     ����keepalive����ʱ���� ��
*/
void sockkpa(const SOCKET &fd, const int32_t &idelay, const int32_t &iintvl);
/*
* \brief          һ���໥���ӵ�socket
* \param sock     SOCKET
* \return         true �ɹ�
*/
bool sockpair(SOCKET sock[2]);

SREY_NS_END

#endif
