#ifndef NETADDR_H_
#define NETADDR_H_

#include "macro.h"

union netaddr_ctx
{
    struct sockaddr addr;
    struct sockaddr_in ipv4;
    struct sockaddr_in6 ipv6;
};

void netaddr_empty_addr(union netaddr_ctx *pctx, const int32_t ifamily);
/*
* \brief          ���õ�ַ
* \param phost    ip
* \param usport   port
* \return         ERR_OK �ɹ� gai_strerror��ȡ������Ϣ
*/
int32_t netaddr_sethost(union netaddr_ctx *pctx, const char *phost, const uint16_t usport);
/*
* \brief          ��ȡԶ�˵�ַ��Ϣ
* \param fd       SOCKET
* \return         ERR_OK �ɹ�
*/
int32_t netaddr_remoteaddr(union netaddr_ctx *pctx, SOCKET fd, const int32_t ifamily);
/*
* \brief          ��ȡ���ص�ַ��Ϣ
* \param fd       SOCKET
* \return         ERR_OK �ɹ�
*/
int32_t netaddr_localaddr(union netaddr_ctx *pctx, SOCKET fd, const int32_t ifamily);
/*
* \brief          ���ص�ַ
* \return         sockaddr *
*/
struct sockaddr *netaddr_addr(union netaddr_ctx *pctx);
/*
* \brief          ��ַ����
* \return         ��ַ����
*/
socklen_t netaddr_size(union netaddr_ctx *pctx);
/*
* \brief          ��ȡIP
* \param acip     ip
* \return         ERR_OK �ɹ�
*/
int32_t netaddr_ip(union netaddr_ctx *pctx, char acip[IP_LENS]);
/*
* \brief          ��ȡ�˿�
* \return         �˿�
*/
uint16_t netaddr_port(union netaddr_ctx *pctx);
/*
* \return         AF_INET or AF_INET6;
*/
int32_t netaddr_family(union netaddr_ctx *pctx);

#endif//NETADDR_H_
