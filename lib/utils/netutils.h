#ifndef NETUTILS_H_
#define NETUTILS_H_

#include "base/macro.h"

void sock_init(void);
void sock_clean(void);
/// <summary>
/// ��ȡsocket�ɶ�����
/// </summary>
/// <param name="fd">socket ���</param>
/// <returns>�ֽ���,ERR_FAILED ʧ��</returns>
int32_t sock_nread(SOCKET fd);
/// <summary>
/// ��ȡsocket������
/// </summary>
/// <param name="fd">socket ���</param>
/// <returns>������,ERR_FAILED ʧ��</returns>
int32_t sock_error(SOCKET fd);
/// <summary>
/// ���socket�Ƿ����ӳɹ�
/// </summary>
/// <param name="fd">socket ���</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t sock_checkconn(SOCKET fd);
/// <summary>
/// ��ȡsocket����
/// </summary>
/// <param name="fd">socket ���</param>
/// <returns>SOCK_STREAM  SOCK_DGRAM, ERR_FAILED ʧ��</returns>
int32_t sock_type(SOCKET fd);
/// <summary>
/// ��ȡsocket��ַ��Ϣ
/// </summary>
/// <param name="fd">socket ���</param>
/// <returns>AF_INET AF_INET6, ERR_FAILED ʧ��</returns>
int32_t sock_family(SOCKET fd);
/// <summary>
/// ������������
/// </summary>
/// <param name="fd">socket ���</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t sock_nodelay(SOCKET fd);
/// <summary>
/// ���÷�����
/// </summary>
/// <param name="fd">socket ���</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t sock_nonblock(SOCKET fd);
/// <summary>
/// ���õ�ַ����
/// </summary>
/// <param name="fd">socket ���</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t sock_reuseaddr(SOCKET fd);
/// <summary>
/// ���ö˿�����
/// </summary>
/// <param name="fd">socket ���</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t sock_reuseport(SOCKET fd);
/// <summary>
/// ����KEEPALIVE
/// </summary>
/// <param name="fd">socket ���</param>
/// <param name="delay">��ú���keepalive ��</param>
/// <param name="intvl">���Լ�� ��</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t sock_keepalive(SOCKET fd, const int32_t delay, const int32_t intvl);
/// <summary>
/// ����SO_LINGER ����TIME_WAIT״̬
/// </summary>
/// <param name="fd">socket ���</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t sock_linger(SOCKET fd);
/// <summary>
/// socket��
/// </summary>
/// <param name="sock">socket����</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t sock_pair(SOCKET sock[2]);

#endif
