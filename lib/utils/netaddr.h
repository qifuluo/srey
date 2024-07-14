#ifndef NETADDR_H_
#define NETADDR_H_

#include "base/macro.h"

typedef union netaddr_ctx {
    struct sockaddr addr;
    struct sockaddr_in ipv4;
    struct sockaddr_in6 ipv6;
}netaddr_ctx;
/// <summary>
/// �Ƿ�Ϊipv4 ��ַ
/// </summary>
/// <param name="ip">IP</param>
/// <returns>ERR_OK ipv4 </returns>
int32_t is_ipv4(const char *ip);
/// <summary>
/// �Ƿ�Ϊipv6 ��ַ
/// </summary>
/// <param name="ip">IP</param>
/// <returns>ERR_OK ipv6 </returns>
int32_t is_ipv6(const char *ip);
/// <summary>
/// �Ƿ�Ϊip��ַ
/// </summary>
/// <param name="ip">IP</param>
/// <returns>ERR_OK ip��ַ </returns>
int32_t is_ipaddr(const char* ip);
/// <summary>
/// ���netaddr_ctx
/// </summary>
/// <param name="ctx">netaddr_ctx</param>
void netaddr_empty(netaddr_ctx *ctx);
/// <summary>
/// ����sockaddr
/// </summary>
/// <param name="ctx">netaddr_ctx</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t netaddr_set(netaddr_ctx *ctx, const char *ip, const uint16_t port);
/// <summary>
/// ��ȡԶ��sockaddr��Ϣ
/// </summary>
/// <param name="ctx">netaddr_ctx</param>
/// <param name="fd">socket���</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t netaddr_remote(netaddr_ctx *ctx, SOCKET fd);
/// <summary>
/// ��ȡ����sockaddr��Ϣ
/// </summary>
/// <param name="ctx">netaddr_ctx</param>
/// <param name="fd">socket���</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t netaddr_local(netaddr_ctx *ctx, SOCKET fd);
/// <summary>
/// ��ȡstruct sockaddr *
/// </summary>
/// <param name="ctx">netaddr_ctx</param>
/// <returns>struct sockaddr *</returns>
struct sockaddr *netaddr_addr(netaddr_ctx *ctx);
/// <summary>
/// ��ȡsockaddr����
/// </summary>
/// <param name="ctx">netaddr_ctx</param>
/// <returns>����</returns>
socklen_t netaddr_size(netaddr_ctx *ctx);
/// <summary>
/// ��ȡip
/// </summary>
/// <param name="ctx">netaddr_ctx</param>
/// <param name="ip">IP</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t netaddr_ip(netaddr_ctx *ctx, char ip[IP_LENS]);
/// <summary>
/// ��ȡ�˿�
/// </summary>
/// <param name="ctx">netaddr_ctx</param>
/// <returns>�˿�</returns>
uint16_t netaddr_port(netaddr_ctx *ctx);
/// <summary>
/// ��ȡAF_INET �� AF_INET6
/// </summary>
/// <param name="ctx">netaddr_ctx</param>
/// <returns>AF_INET �� AF_INET6</returns>
int32_t netaddr_family(netaddr_ctx *ctx);

#endif//NETADDR_H_
