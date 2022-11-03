#ifndef NETUTILS_H_
#define NETUTILS_H_

#include "macro.h"

void sock_init();
void sock_clean();
//��ȡsocket�ɶ�����
int32_t sock_nread(SOCKET fd);
//��ȡSO_TYPE(SOCK_STREAM  SOCK_DGRAM)
int32_t sock_type(SOCKET fd);
//��ȡsin_family(AF_INET AF_INET6)
int32_t sock_family(SOCKET fd);
//����TCP_NODELAY
void sock_nodelay(SOCKET fd);
//������
void sock_nbio(SOCKET fd);
//��ַ���� SO_REUSEADDR
void sock_raddr(SOCKET fd);
//�Ƿ�֧�ֶ˿�����
int32_t sock_checkrport();
//�˿�����
void sock_rport(SOCKET fd);
//SO_KEEPALIVE
void sock_kpa(SOCKET fd, const int32_t delay, const int32_t intvl);
//����SO_LINGER ������TIME_WAIT״̬
void sock_linger(SOCKET fd);
//һ���໥���ӵ�socket
int32_t sock_pair(SOCKET sock[2]);

#endif
