#ifndef CORO_H_
#define CORO_H_

#include "srey/spub.h"

#if WITH_CORO
/// <summary>
/// ����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="ms">����</param>
void coro_sleep(task_ctx *task, uint32_t ms);
/// <summary>
/// �����ͨ�� ����
/// </summary>
/// <param name="dst">Ŀ������</param>
/// <param name="src">������</param>
/// <param name="rtype">��������</param>
/// <param name="data">����</param>
/// <param name="size">���ݳ���</param>
/// <param name="copy">1 �������� 0 ����������</param>
/// <param name="erro">������</param>
/// <param name="lens">�������ݳ���</param>
/// <returns>����</returns>
void *coro_request(task_ctx *dst, task_ctx *src, uint8_t rtype, void *data, size_t size, int32_t copy, int32_t *erro, size_t *lens);
/// <summary>
/// �л�ΪSSL����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="client">1 ��Ϊ�ͻ��� 0 ��Ϊ�����</param>
/// <param name="evssl">evssl_ctx</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t coro_ssl_exchange(task_ctx *task, SOCKET fd, uint64_t skid, int32_t client, struct evssl_ctx *evssl);
/// <summary>
/// �ȴ��������
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="err">������</param>
/// <param name="size">�������ݳ���</param>
/// <returns>����</returns>
void *coro_handshaked(task_ctx *task, SOCKET fd, uint64_t skid, int32_t *err, size_t *size);
/// <summary>
/// ����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="pktype">���ݰ�����</param>
/// <param name="evssl">evssl_ctx</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="skid">����ID</param>
/// <param name="netev">task_netev</param>
/// <returns>socket���</returns>
SOCKET coro_connect(task_ctx *task, pack_type pktype, struct evssl_ctx *evssl, const char *ip, uint16_t port, uint64_t *skid, int32_t netev);
/// <summary>
/// TCP����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="data">����</param>
/// <param name="len">���ݳ���</param>
/// <param name="size">�������ݳ���</param>
/// <param name="copy">1 �������� 0 ������</param>
/// <returns>����</returns>
void *coro_send(task_ctx *task, SOCKET fd, uint64_t skid, void *data, size_t len, size_t *size, int32_t copy);
/// <summary>
/// �ȴ���Ƭ��Ϣ
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="size">���ݳ���</param>
/// <param name="end">1 ��Ƭ���� 0δ����</param>
/// <returns>����</returns>
void *coro_slice(task_ctx *task, SOCKET fd, uint64_t skid, size_t *size, int32_t *end);
/// <summary>
/// UDP����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="data">����</param>
/// <param name="len">���ݳ���</param>
/// <param name="size">�������ݳ���</param>
/// <returns>����</returns>
void *coro_sendto(task_ctx *task, SOCKET fd, uint64_t skid, const char *ip, const uint16_t port, void *data, size_t len, size_t *size);

#endif
#endif//CORO_H_
