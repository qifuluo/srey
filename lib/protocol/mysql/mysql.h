#ifndef MYSQL_H_
#define MYSQL_H_

#include "protocol/mysql/mysql_struct.h"
#include "event/event.h"
#include "srey/spub.h"

void _mysql_pkfree(void *pack);
void _mysql_init(void *hspush);
void _mysql_udfree(ud_cxt *ud);
void _mysql_closed(ud_cxt *ud);
int32_t _mysql_ssl_exchanged(ev_ctx *ev, ud_cxt *ud);
//mysql ��֤ ���ݽ��
void *mysql_unpack(ev_ctx *ev, buffer_ctx *buf, ud_cxt *ud, int32_t *status);
/// <summary>
/// mysql������ʼ��
/// </summary>
/// <param name="mysql">mysql_ctx</param>
/// <param name="ip">IP</param>
/// <param name="port">�˿� 3306</param>
/// <param name="evssl">evssl_ctx ��NULL ����SSL����</param>
/// <param name="user">�û���</param>
/// <param name="password">����</param>
/// <param name="database">���ݿ�, NULL �� "" ������</param>
/// <param name="charset">�����ʽ</param>
/// <param name="maxpk">������ݰ�, 0: ONEK * ONEK</param>
/// <param name="relink">1 ping��ʱ���Զ�����</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t mysql_init(mysql_ctx *mysql, const char *ip, uint16_t port, struct evssl_ctx *evssl,
    const char *user, const char *password, const char *database, const char *charset, uint32_t maxpk, int32_t relink);
/// <summary>
/// �������ݿ�
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="mysql">mysql_ctx</param>
/// <returns>ERR_OK ����ɹ�</returns>
int32_t mysql_try_connect(task_ctx *task, mysql_ctx *mysql);
/// <summary>
/// ��ȡ�汾��Ϣ
/// </summary>
/// <param name="mysql">mysql_ctx</param>
/// <returns>�汾��Ϣ</returns>
const char *mysql_version(mysql_ctx *mysql);
/// <summary>
/// ��ȡ������Ϣ
/// </summary>
/// <param name="mysql">mysql_ctx</param>
/// <param name="code">������, NULL������</param>
/// <returns>������Ϣ</returns>
const char *mysql_erro(mysql_ctx *mysql, int32_t *code);
/// <summary>
/// ���������Ϣ
/// </summary>
/// <param name="mysql">mysql_ctx</param>
void mysql_erro_clear(mysql_ctx *mysql);
/// <summary>
/// ��ȡ�����ID
/// </summary>
/// <param name="mysql">mysql_ctx</param>
/// <returns>ID</returns>
int64_t mysql_last_id(mysql_ctx *mysql);
/// <summary>
/// ��ȡӰ�������
/// </summary>
/// <param name="mysql">mysql_ctx</param>
/// <returns>Ӱ�������</returns>
int64_t mysql_affected_rows(mysql_ctx *mysql);
/// <summary>
/// stmt �ر�
/// </summary>
/// <param name="task">task_ctx</param>
/// <param name="stmt">mysql_stmt_ctx</param>
void mysql_stmt_close(task_ctx *task, mysql_stmt_ctx *stmt);

#endif//MYSQL_H_
