#ifndef CORO_UTILS_H_
#define CORO_UTILS_H_

#include "srey/spub.h"
#include "protocol/mysql/mysql.h"

#if WITH_CORO
/// <summary>
/// dns��������
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="domain">����</param>
/// <param name="ipv6">1 ipv6 0 ipv4</param>
/// <param name="cnt">ip����</param>
/// <returns>dns_ip ��ҪFREE</returns>
struct dns_ip *dns_lookup(task_ctx *task, const char *domain, int32_t ipv6, size_t *cnt);
/// <summary>
/// websocket����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="evssl">evssl_ctx</param>
/// <param name="ws">ws://host:port</param>
/// <param name="secproto">Sec-WebSocket-Protocol</param>
/// <param name="skid">����ID</param>
/// <param name="netev">task_netev</param>
/// <returns>socket���</returns>
SOCKET wbsock_connect(task_ctx *task, struct evssl_ctx *evssl, const char *ws, const char *secproto, uint64_t *skid, int32_t netev);
/// <summary>
/// redis����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="evssl">evssl_ctx</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿�</param>
/// <param name="key">����</param>
/// <param name="skid">����ID</param>
/// <param name="netev">task_netev</param>
/// <returns>socket���</returns>
SOCKET redis_connect(task_ctx *task, struct evssl_ctx *evssl, const char *ip, uint16_t port, const char *key, uint64_t *skid, int32_t netev);
/// <summary>
/// myql����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="mysql">mysql_ctx, mysql_init</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t mysql_connect(task_ctx *task, mysql_ctx *mysql);
/// <summary>
/// ѡ�����ݿ�
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="mysql">mysql_ctx</param>
/// <param name="mysql">���ݿ�</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t mysql_selectdb(task_ctx *task, mysql_ctx *mysql, const char *database);
/// <summary>
/// ping
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="mysql">mysql_ctx</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t mysql_ping(task_ctx *task, mysql_ctx *mysql);
/// <summary>
/// ִ��SQL���
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="mysql">mysql_ctx</param>
/// <param name="sql">SQL���</param>
/// <param name="mbind">mysql_bind_ctx</param>
/// <returns>mpack_ctx NULL ʧ��</returns>
mpack_ctx *mysql_query(task_ctx *task, mysql_ctx *mysql, const char *sql, mysql_bind_ctx *mbind);
/// <summary>
/// Ԥ����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="mysql">mysql_ctx</param>
/// <param name="sql">SQL���</param>
/// <returns>mysql_stmt_ctx NULL ʧ��</returns>
mysql_stmt_ctx *mysql_stmt_prepare(task_ctx *task, mysql_ctx *mysql, const char *sql);
/// <summary>
/// Ԥ����ִ��
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="stmt">mysql_stmt_ctx</param>
/// <param name="mbind">mysql_bind_ctx</param>
/// <returns>mpack_ctx NULL ʧ��</returns>
mpack_ctx *mysql_stmt_execute(task_ctx *task, mysql_stmt_ctx *stmt, mysql_bind_ctx *mbind);
/// <summary>
/// Ԥ��������
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="stmt">mysql_stmt_ctx</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t mysql_stmt_reset(task_ctx *task, mysql_stmt_ctx *stmt);
/// <summary>
/// �˳��ر�����
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="mysql">mysql_ctx</param>
void mysql_quit(task_ctx *task, mysql_ctx *mysql);

#endif
#endif//CORO_UTILS_H_
