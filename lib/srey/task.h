#ifndef TASK_H_
#define TASK_H_

#include "srey/spub.h"

typedef enum task_netev {
    NETEV_NONE = 0x00,
    NETEV_ACCEPT = 0x01,
    NETEV_AUTHSSL = 0x02,
    NETEV_SEND = 0x04
}task_netev;
/// <summary>
/// �½�����
/// </summary>
/// <param name="loader">loader_ctx</param>
/// <param name="name">������</param>
/// <param name="_dispatch">��Ϣ�ַ�����, NULLĬ�Ϸַ�����</param>
/// <param name="_argfree">�û������ͷź���</param>
/// <param name="arg">�û�����</param>
/// <returns>task_ctx</returns>
task_ctx *task_new(loader_ctx *loader, name_t name, _task_dispatch_cb _dispatch, free_cb _argfree, void *arg);
/// <summary>
/// �����ͷ�
/// </summary>
/// <param name="task">task_ctx</param>
void task_free(task_ctx *task);
/// <summary>
/// ����ע��
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="_startup">�����ʼ���ص�����</param>
/// <param name="_closing">�����ͷŻص�����</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t task_register(task_ctx *task, _task_startup_cb _startup, _task_closing_cb _closing);
/// <summary>
/// ����ر� MSG_TYPE_CLOSING
/// </summary>
/// <param name="task">task_ctx</param>
void task_close(task_ctx *task);
/// <summary>
/// ��ȡ����,��ʹ���ü�һ
/// </summary>
/// <param name="loader">loader_ctx</param>
/// <param name="name">������</param>
/// <returns>task_ctx NULLδ�ҵ�����</returns>
task_ctx *task_grab(loader_ctx *loader, name_t name);
/// <summary>
/// �������ü�һ
/// </summary>
/// <param name="task">task_ctx</param>
void task_incref(task_ctx *task);
/// <summary>
/// �ͷ�task_grab��ȡ������,��ʹ���ü�һ. �� task_grab task_incref ���
/// </summary>
/// <param name="task">task_ctx</param>
void task_ungrab(task_ctx *task);
/// <summary>
/// ��ʱ
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="sess">session</param>
/// <param name="ms">����</param>
/// <param name="_timeout">��ʱ�ص�����</param>
void task_timeout(task_ctx *task, uint64_t sess, uint32_t ms, _timeout_cb _timeout);
/// <summary>
/// �����ͨ��,����
/// </summary>
/// <param name="dst">Ŀ��task_ctx</param>
/// <param name="src">����task_ctx</param>
/// <param name="reqtype">����</param>
/// <param name="sess">session</param>
/// <param name="data">����</param>
/// <param name="size">���ݳ���</param>
/// <param name="copy">1 ���� 0������</param>
void task_request(task_ctx *dst, task_ctx *src, uint8_t reqtype, uint64_t sess, void *data, size_t size, int32_t copy);
/// <summary>
/// �����ͨ��,����
/// </summary>
/// <param name="dst">Ŀ��task_ctx</param>
/// <param name="sess">session</param>
/// <param name="erro">������</param>
/// <param name="data">����</param>
/// <param name="size">���ݳ���</param>
/// <param name="copy">1 ���� 0������</param>
void task_response(task_ctx *dst, uint64_t sess, int32_t erro, void *data, size_t size, int32_t copy);
/// <summary>
/// �����ͨ��,�޷���
/// </summary>
/// <param name="dst">Ŀ��task_ctx</param>
/// <param name="reqtype">����</param>
/// <param name="data">����</param>
/// <param name="size">���ݳ���</param>
/// <param name="copy">1 ���� 0������</param>
void task_call(task_ctx *dst, uint8_t reqtype, void *data, size_t size, int32_t copy);
/// <summary>
/// ����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="pktype">������</param>
/// <param name="evssl">evssl_ctx</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="id">����ID</param>
/// <param name="netev">task_netev</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t task_listen(task_ctx *task, pack_type pktype, struct evssl_ctx *evssl,
    const char *ip, uint16_t port, uint64_t *id, int32_t netev);
/// <summary>
/// ����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="pktype">������</param>
/// <param name="evssl">evssl_ctx</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="skid">����ID</param>
/// <param name="netev">task_netev</param>
/// <returns>socket���</returns>
SOCKET task_connect(task_ctx *task, pack_type pktype, struct evssl_ctx *evssl,
    const char *ip, uint16_t port, uint64_t *skid, int32_t netev);
/// <summary>
/// ����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="pktype">������</param>
/// <param name="extra">ud_cxt extra</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="skid">����ID</param>
/// <param name="netev">task_netev</param>
/// <returns>socket���</returns>
SOCKET task_conn_extra(task_ctx *task, pack_type pktype, void *extra,
    const char *ip, uint16_t port, uint64_t *skid, int32_t netev);
/// <summary>
/// UDP
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="skid">����ID</param>
/// <returns>socket���</returns>
SOCKET task_udp(task_ctx *task, const char *ip, uint16_t port, uint64_t *skid);
//ע��ص�
void on_accepted(task_ctx *task, _net_accept_cb _accept);
void on_recved(task_ctx *task, _net_recv_cb _recv);
void on_sended(task_ctx *task, _net_send_cb _send);
void on_connected(task_ctx *task, _net_connect_cb _connect);
void on_ssl_exchanged(task_ctx *task, _net_ssl_exchanged_cb _exchanged);
void on_handshaked(task_ctx *task, _net_handshake_cb _handshake);
void on_closed(task_ctx *task, _net_close_cb _close);
void on_recvedfrom(task_ctx *task, _net_recvfrom_cb _recvfrom);
void on_requested(task_ctx *task, _request_cb _request);
void on_responsed(task_ctx *task, _response_cb _response);

#endif//TASK_H_
