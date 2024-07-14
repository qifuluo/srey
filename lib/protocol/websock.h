#ifndef WEBSOCK_H_
#define WEBSOCK_H_

#include "event/event.h"

typedef enum  websock_proto {
    WBSK_CONTINUE = 0x00,
    WBSK_TEXT = 0x01,
    WBSK_BINARY = 0x02,
    WBSK_CLOSE = 0x08,
    WBSK_PING = 0x09,
    WBSK_PONG = 0x0A
}websock_proto;

void _websock_pkfree(void *data);
//���
struct websock_pack_ctx *websock_unpack(ev_ctx *ev, SOCKET fd, uint64_t skid, int32_t client,
    buffer_ctx *buf, ud_cxt *ud, int32_t *status);
/// <summary>
/// ���ְ�
/// </summary>
/// <param name="host">Host</param>
/// <param name="secproto">Sec-WebSocket-Protocol</param>
/// <returns>���ְ�</returns>
char *websock_handshake_pack(const char *host, const char *secproto);
/// <summary>
/// ping��
/// </summary>
/// <param name="mask">1 ����, �ͻ�����������������ݶ���Ҫ����, 0 ������</param>
/// <param name="size">������</param>
/// <returns>ping��</returns>
void *websock_ping(int32_t mask, size_t *size);
/// <summary>
/// pong��
/// </summary>
/// <param name="mask">1 ����, �ͻ�����������������ݶ���Ҫ����, 0 ������</param>
/// <param name="size">������</param>
/// <returns>pong��</returns>
void *websock_pong(int32_t mask, size_t *size);
/// <summary>
/// close��
/// </summary>
/// <param name="mask">1 ����, �ͻ�����������������ݶ���Ҫ����, 0 ������</param>
/// <param name="size">������</param>
/// <returns>close��</returns>
void *websock_close(int32_t mask, size_t *size);
/// <summary>
/// �ı���Ϣ��
/// </summary>
/// <param name="mask">1 ����, �ͻ�����������������ݶ���Ҫ����, 0 ������</param>
/// <param name="fin">1 ������ 0 ��Ƭ</param>
/// <param name="data">����</param>
/// <param name="dlens">���ݳ���</param>
/// <param name="size">������</param>
/// <returns>�ı���Ϣ��</returns>
void *websock_text(int32_t mask, int32_t fin, void *data, size_t dlens, size_t *size);
/// <summary>
/// ��������Ϣ��
/// </summary>
/// <param name="mask">1 ����, �ͻ�����������������ݶ���Ҫ����, 0 ������</param>
/// <param name="fin">1 ������ 0 ��Ƭ</param>
/// <param name="data">����</param>
/// <param name="dlens">���ݳ���</param>
/// <param name="size">������</param>
/// <returns>��������Ϣ��</returns>
void *websock_binary(int32_t mask, int32_t fin, void *data, size_t dlens, size_t *size);
/// <summary>
/// ��Ƭ��Ϣ��
/// </summary>
/// <param name="mask">1 ����, �ͻ�����������������ݶ���Ҫ����, 0 ������</param>
/// <param name="fin">1 ���� 0 δ����</param>
/// <param name="data">����</param>
/// <param name="dlens">���ݳ���</param>
/// <param name="size">������</param>
/// <returns>��Ƭ��Ϣ��</returns>
void *websock_continuation(int32_t mask, int32_t fin, void *data, size_t dlens, size_t *size);
/// <summary>
/// ��ȡfinֵ
/// </summary>
/// <param name="pack">websock_pack_ctx</param>
/// <returns>fin</returns>
int32_t websock_pack_fin(struct websock_pack_ctx *pack);
/// <summary>
/// ��ȡЭ���
/// </summary>
/// <param name="pack">websock_pack_ctx</param>
/// <returns>Э���</returns>
int32_t websock_pack_proto(struct websock_pack_ctx *pack);
/// <summary>
/// ��ȡ����
/// </summary>
/// <param name="pack">websock_pack_ctx</param>
/// <param name="pack">���ݳ���</param>
/// <returns>����</returns>
char *websock_pack_data(struct websock_pack_ctx *pack, size_t *lens);

void _websock_init(void *hspush);

#endif//WEBSOCK_H_
