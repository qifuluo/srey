#ifndef DNS_H_
#define DNS_H_

#include "base/structs.h"

typedef struct dns_ip {
    char ip[IP_LENS];
}dns_ip;
/// <summary>
/// dns�������������
/// </summary>
/// <param name="buf">�����, buf������0</param>
/// <param name="domain">Ҫ����������</param>
/// <param name="ipv6">1 ipv6 0 ipv4</param>
/// <returns>���������</returns>
size_t dns_request_pack(char *buf, const char *domain, int32_t ipv6);
/// <summary>
/// ����dns��������
/// </summary>
/// <param name="buf">dns �������ݰ�</param>
/// <param name="cnt">����</param>
/// <returns>dns_ip ��ҪFREE</returns>
dns_ip *dns_parse_pack(char *buf, size_t *cnt);
/// <summary>
/// ����dns������IP
/// </summary>
/// <param name="ip">ip</param>
void dns_set_ip(const char *ip);
/// <summary>
/// ��ȡ���õ�dns������IP
/// </summary>
/// <returns>ip</returns>
const char *dns_get_ip(void);

#endif//DNS_H_
