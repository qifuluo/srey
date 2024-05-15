#ifndef DNS_H_
#define DNS_H_

#include "structs.h"

typedef struct dns_ip {
    char ip[IP_LENS];
}dns_ip;

//buf������0
size_t dns_request_pack(char *buf, const char *domain, int32_t ipv6);
//��ҪFREE
dns_ip *dns_parse_pack(char *buf, size_t *cnt);
void dns_set_ip(const char *ip);
const char *dns_get_ip(void);

#endif//DNS_H_
