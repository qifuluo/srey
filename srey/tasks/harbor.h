#ifndef HARBOR_H_
#define HARBOR_H_

#include "lib.h"

/// <summary>
/// ����harbor,��������ͨ��
/// </summary>
/// <param name="loader">loader_ctx</param>
/// <param name="tname">������</param>
/// <param name="ssl">evssl_ctx ����</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="key">��Կ</param>
/// <param name="ms">�����ͨ�ų�ʱʱ�� ����</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t harbor_start(loader_ctx *loader, name_t tname, name_t ssl,
    const char *ip, uint16_t port, const char *key, int32_t ms);
/// <summary>
/// ��������ͨ�������
/// </summary>
/// <param name="task">Ŀ��������</param>
/// <param name="call">1 ִ��call 0 ִ��request</param>
/// <param name="reqtype">��������</param>
/// <param name="key">��Կ</param>
/// <param name="data">����</param>
/// <param name="size">���ݳ���</param>
/// <param name="lens">���������</param>
/// <returns>�����</returns>
void *harbor_pack(name_t task, int32_t call, uint8_t reqtype, const char *key, void *data, size_t size, size_t *lens);

#endif//HARBOR_H_
