#ifndef NETUTILS_H_
#define NETUTILS_H_

#include "macro.h"

/*
* \brief          ��ȡsocket�ɶ�����
* \param fd       socket���
* \return         ERR_FAILED ʧ��
* \return         ����
*/
int32_t socknread(SOCKET fd);
/*
* \brief          ��ȡsa_family
* \param fd       SOCKET
* \return         ERR_FAILED ʧ��
* \return         SOCK_STREAM  SOCK_DGRAM
*/
int32_t socktype(SOCKET fd);
/*
* \brief          ��ȡsin_family
* \param fd       SOCKET
* \return         ERR_FAILED ʧ��
*/
int32_t sockaddrfamily(SOCKET fd);
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
