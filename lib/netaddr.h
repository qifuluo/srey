#ifndef NETADDR_H_
#define NETADDR_H_

#include "macro.h"

#define  IPV4 0
#define  IPV6 1
struct netaddr_ctx
{
    int32_t m_type;
    struct sockaddr_in	m_ipv4;
    struct sockaddr_in6 m_ipv6;
}netaddr_ctx;

/*
* \brief          ���õ�ַ
* \param phost    ip
* \param usport   port
* \param bipv6    �Ƿ�IP V6
* \return         ERR_OK �ɹ�
*/
int32_t netaddr_sethost(struct netaddr_ctx *pctx, const char *phost, const uint16_t usport);
/*
* \brief          ��ȡԶ�˵�ַ��Ϣ
* \param fd       SOCKET
* \return         ERR_OK �ɹ�
*/
int32_t netaddr_remoteaddr(struct netaddr_ctx *pctx, const SOCKET fd);
/*
* \brief          ��ȡ���ص�ַ��Ϣ
* \param fd       SOCKET
* \return         ERR_OK �ɹ�
*/
int32_t netaddr_localaddr(struct netaddr_ctx *pctx, const SOCKET fd);
/*
* \brief          ���ص�ַ
* \return         sockaddr *
*/
struct sockaddr *netaddr_addr(struct netaddr_ctx *pctx);
/*
* \brief          ��ַ����
* \return         ��ַ����
*/
socklen_t netaddr_size(struct netaddr_ctx *pctx);
/*
* \brief          ��ȡIP
* \param acip     ip
* \return         ERR_OK �ɹ�
*/
int32_t netaddr_ip(struct netaddr_ctx *pctx, char acip[IP_LENS]);
/*
* \brief          ��ȡ�˿�
* \return         �˿�
*/
uint16_t netaddr_port(struct netaddr_ctx *pctx);
/*
* \brief          �Ƿ�Ϊipv4
* \return         ERR_OK ��
*/
int32_t netaddr_isipv4(struct netaddr_ctx *pctx);
/*
* \return         AF_INET or AF_INET6;
*/
int32_t netaddr_addrfamily(struct netaddr_ctx *pctx);

#endif//NETADDR_H_
