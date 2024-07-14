#ifndef MYSQL_READER_H_
#define MYSQL_READER_H_

#include "protocol/mysql/mysql.h"

/// <summary>
/// mysql_reader ��ʼ��
/// </summary>
/// <param name="mpack">mpack_ctx</param>
/// <returns>mysql_reader_ctx NULL ʧ��</returns>
mysql_reader_ctx *mysql_reader_init(mpack_ctx *mpack);
/// <summary>
/// mysql_reader �ͷ�
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
void mysql_reader_free(mysql_reader_ctx *reader);
/// <summary>
/// �ж���������
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
/// <returns>��������</returns>
size_t mysql_reader_size(mysql_reader_ctx *reader);
/// <summary>
/// �ڼ�������
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
/// <param name="pos">����</param>
void mysql_reader_seek(mysql_reader_ctx *reader, size_t pos);
/// <summary>
/// �Ƿ�������
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
/// <returns>1 ������ 0 ������</returns>
int32_t mysql_reader_eof(mysql_reader_ctx *reader);
/// <summary>
/// ��һ������
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
void mysql_reader_next(mysql_reader_ctx *reader);
/// <summary>
/// �������ƻ�ȡ�ֶ�����ֵ
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
/// <param name="name">�ֶ�</param>
/// <param name="err">ERR_OK �ɹ�  ERR_FAILED ʧ�� 1 nil</param>
/// <returns>ֵ</returns>
int64_t mysql_reader_integer(mysql_reader_ctx *reader, const char *name, int32_t *err);
/// <summary>
/// �������ƻ�ȡ�ֶ��޷�������ֵ
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
/// <param name="name">�ֶ�</param>
/// <param name="err">ERR_OK �ɹ�  ERR_FAILED ʧ�� 1 nil</param>
/// <returns>ֵ</returns>
uint64_t mysql_reader_uinteger(mysql_reader_ctx *reader, const char *name, int32_t *err);
/// <summary>
/// �������ƻ�ȡ�ֶθ�����ֵ
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
/// <param name="name">�ֶ�</param>
/// <param name="err">ERR_OK �ɹ�  ERR_FAILED ʧ�� 1 nil</param>
/// <returns>ֵ</returns>
double mysql_reader_double(mysql_reader_ctx *reader, const char *name, int32_t *err);
/// <summary>
/// �������ƻ�ȡ�ֶ��ַ���ֵ
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
/// <param name="name">�ֶ�</param>
/// <param name="lens">����</param>
/// <param name="err">ERR_OK �ɹ�  ERR_FAILED ʧ�� 1 nil</param>
/// <returns>ֵ</returns>
char *mysql_reader_string(mysql_reader_ctx *reader, const char *name, size_t *lens, int32_t *err);
/// <summary>
/// �������ƻ�ȡ�ֶ�ʱ���ֵ
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
/// <param name="name">�ֶ�</param>
/// <param name="err">ERR_OK �ɹ�  ERR_FAILED ʧ�� 1 nil</param>
/// <returns>ʱ���</returns>
uint64_t mysql_reader_datetime(mysql_reader_ctx *reader, const char *name, int32_t *err);
/// <summary>
/// �������ƻ�ȡ�ֶ�ʱ��ֵ
/// </summary>
/// <param name="reader">mysql_reader_ctx</param>
/// <param name="name">�ֶ�</param>
/// <param name="time">struct tm</param>
/// <param name="err">ERR_OK �ɹ�  ERR_FAILED ʧ�� 1 nil</param>
/// <returns>1�� 0 ��</returns>
int32_t mysql_reader_time(mysql_reader_ctx *reader, const char *name, struct tm *time, int32_t *err);

#endif//MYSQL_READER_H_
