#ifndef EVENT_H_ 
#define EVENT_H_

#include "event/evpub.h"
#include "event/evssl.h"

/// <summary>
/// �����ʼ��
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="nthreads">�߳���</param>
void ev_init(ev_ctx *ctx, uint32_t nthreads);
/// <summary>
/// �����ͷ�
/// </summary>
/// <param name="ctx">ev_ctx</param>
void ev_free(ev_ctx *ctx);
/// <summary>
/// ����
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="evssl">evssl_ctx, NULL ��ʹ��</param>
/// <param name="ip">����IP(0.0.0.0 - ::  127.0.0.1 - ::1)</param>
/// <param name="port">�����˿�</param>
/// <param name="cbs">�ص�����</param>
/// <param name="ud">�û�����</param>
/// <param name="id">����ID</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t ev_listen(ev_ctx *ctx, struct evssl_ctx *evssl, const char *ip, const uint16_t port,
    cbs_ctx *cbs, ud_cxt *ud, uint64_t *id);
/// <summary>
/// ����
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="evssl">evssl_ctx, NULL ��ʹ��</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="cbs">�ص�����</param>
/// <param name="ud">�û�����</param>
/// <param name="skid">����ID</param>
/// <returns>socket���</returns>
SOCKET ev_connect(ev_ctx *ctx, struct evssl_ctx *evssl, const char *ip, const uint16_t port,
    cbs_ctx *cbs, ud_cxt *ud, uint64_t *skid);
/// <summary>
/// �л�ΪSSL����
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="client">1 ��Ϊ�ͻ���, 0 ��Ϊ�����</param>
/// <param name="evssl">evssl_ctx</param>
void ev_ssl(ev_ctx *ctx, SOCKET fd, uint64_t skid, int32_t client, struct evssl_ctx *evssl);
/// <summary>
/// UDP
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="cbs">�ص�����</param>
/// <param name="ud">�û�����</param>
/// <param name="skid">����ID</param>
/// <returns>socket���</returns>
SOCKET ev_udp(ev_ctx *ctx, const char *ip, const uint16_t port, cbs_ctx *cbs, ud_cxt *ud, uint64_t *skid);
/// <summary>
/// TCP��������
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="data">Ҫ���͵�����</param>
/// <param name="len">���ݳ���</param>
/// <param name="copy">1 ��������, 0����������</param>
void ev_send(ev_ctx *ctx, SOCKET fd, uint64_t skid, void *data, size_t len, int32_t copy);
/// <summary>
/// UDP��������
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="data">Ҫ���͵�����</param>
/// <param name="len">���ݳ���</param>
/// <returns>ERR_OK ����ɹ�</returns>
int32_t ev_sendto(ev_ctx *ctx, SOCKET fd, uint64_t skid, const char *ip, const uint16_t port, void *data, size_t len);
/// <summary>
/// �ر�����
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
void ev_close(ev_ctx *ctx, SOCKET fd, uint64_t skid);
/// <summary>
/// ȡ������
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="id">����ID</param>
void ev_unlisten(ev_ctx *ctx, uint64_t id);
/// <summary>
/// ����ud_cxt�����ݰ�����
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="pktype">���ݰ����� pack_type</param>
void ev_ud_pktype(ev_ctx *ctx, SOCKET fd, uint64_t skid, uint8_t pktype);
/// <summary>
/// ����ud_cxt��״̬
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="status">״̬</param>
void ev_ud_status(ev_ctx *ctx, SOCKET fd, uint64_t skid, uint8_t status);
/// <summary>
/// ����ud_cxt��session
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="sess">session</param>
void ev_ud_sess(ev_ctx *ctx, SOCKET fd, uint64_t skid, uint64_t sess);
/// <summary>
/// ����ud_cxt��������
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="name">������</param>
void ev_ud_name(ev_ctx *ctx, SOCKET fd, uint64_t skid, name_t name);
/// <summary>
/// ����ud_cxt��extra
/// </summary>
/// <param name="ctx">ev_ctx</param>
/// <param name="fd">socket���</param>
/// <param name="skid">����ID</param>
/// <param name="extra">extra</param>
void ev_ud_extra(ev_ctx *ctx, SOCKET fd, uint64_t skid, void *extra);

#endif//EVENT_H_
