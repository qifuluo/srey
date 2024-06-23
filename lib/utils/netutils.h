#ifndef NETUTILS_H_
#define NETUTILS_H_

#include "base/macro.h"

void sock_init(void);
void sock_clean(void);
//��ȡsocket�ɶ�����
int32_t sock_nread(SOCKET fd);
//����״̬
int32_t sock_error(SOCKET fd);
int32_t sock_checkconn(SOCKET fd);
//��ȡSO_TYPE(SOCK_STREAM  SOCK_DGRAM)
int32_t sock_type(SOCKET fd);
//��ȡsin_family(AF_INET AF_INET6)
int32_t sock_family(SOCKET fd);
//����TCP_NODELAY
int32_t sock_nodelay(SOCKET fd);
//������
int32_t sock_nonblock(SOCKET fd);
//��ַ���� SO_REUSEADDR
int32_t sock_reuseaddr(SOCKET fd);
//�˿�����
int32_t sock_reuseport(SOCKET fd);
//SO_KEEPALIVE
int32_t sock_keepalive(SOCKET fd, const int32_t delay, const int32_t intvl);
//����SO_LINGER ����TIME_WAIT״̬
int32_t sock_linger(SOCKET fd);
//һ���໥���ӵ�socket
int32_t sock_pair(SOCKET sock[2]);

#endif
