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
int32_t socknread(const SOCKET &fd);
/*
* \brief          socket�xȡ����
* \param fd       socket���
* \return         ERR_FAILED ʧ�ܣ���Ҫ�P�]socket
* \return         ����
*/
//int32_t sockrecv(const SOCKET &fd, class cchainbuffer *pbuf);
/*
* \brief          ����һ����socket
* \param ip       ip
* \param port     port
* \param backlog  �ȴ����Ӷ��е���󳤶� -1 ʹ��128
* \return         INVALID_SOCK ʧ��
*/
SOCKET socklsn(const char *ip, const uint16_t &port, const int32_t &backlog);
/*
* \brief          ����һsocket����
* \param ip       ip
* \param port     port
* \return         INVALID_SOCK ʧ��
*/
SOCKET sockcnt(const char *ip, const uint16_t &port);
/*
* \brief          ����socket���� TCP_NODELAY  SO_KEEPALIVE ������
* \param fd       SOCKET
*/
void sockopts(SOCKET &fd);
/*
* \brief          һ���໥���ӵ�socket
* \param sock     SOCKET
* \return         true �ɹ�
*/
bool sockpair(SOCKET sock[2]);

SREY_NS_END

#endif
