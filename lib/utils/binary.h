#ifndef BINARY_H_
#define BINARY_H_

#include "base/structs.h"
#include "utils/utils.h"

#define BINARY_INCREASE 256

typedef struct binary_ctx {
    char *data;
    size_t inc;
    size_t size;
    size_t offset;
}binary_ctx;
/// <summary>
/// �����ڴ��д��ʼ��
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="buf">ֵ</param>
/// <param name="lens">����</param>
/// <param name="inc">���ӻ���</param>
void binary_init(binary_ctx *ctx, char *buf, size_t lens, size_t inc);
/// <summary>
/// ����ƫ��ֵ
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="off">ƫ��</param>
void binary_offset(binary_ctx *ctx, size_t off);
/// <summary>
/// д��int8
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="val">ֵ</param>
void binary_set_int8(binary_ctx *ctx, int8_t val);
/// <summary>
/// д��uint8
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="val">ֵ</param>
void binary_set_uint8(binary_ctx *ctx, uint8_t val);
/// <summary>
/// д������
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="val">ֵ</param>
/// <param name="lens">val�ֽ���</param>
/// <param name="islittle">1 С������ 0�������</param>
void binary_set_integer(binary_ctx *ctx, int64_t val, size_t lens, int32_t islittle);
/// <summary>
/// д���޷�������
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="val">ֵ</param>
/// <param name="lens">val�ֽ���</param>
/// <param name="islittle">1 С������ 0�������</param>
void binary_set_uinteger(binary_ctx *ctx, uint64_t val, size_t lens, int32_t islittle);
/// <summary>
/// д��float
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="val">ֵ</param>
/// <param name="islittle">1 С������ 0�������</param>
void binary_set_float(binary_ctx *ctx, float val, int32_t islittle);
/// <summary>
/// д��double
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="val">ֵ</param>
/// <param name="islittle">1 С������ 0�������</param>
void binary_set_double(binary_ctx *ctx, double val, int32_t islittle);
/// <summary>
/// д��char *
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="buf">ֵ</param>
/// <param name="lens">����</param>
void binary_set_string(binary_ctx *ctx, const char *buf, size_t lens);
/// <summary>
/// ���
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="val">�Ը�ֵ���</param>
/// <param name="lens">��䳤��</param>
void binary_set_fill(binary_ctx *ctx, char val, size_t lens);
/// <summary>
/// ����ָ������
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="lens">����</param>
void binary_set_skip(binary_ctx *ctx, size_t lens);
/// <summary>
/// д��������
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="fmt">��ʽ��</param>
/// <param name="...">���</param>
void binary_set_va(binary_ctx *ctx, const char *fmt, ...);
/// <summary>
/// ��ȡָ��λ�õ�ָ��
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="pos">λ��</param>
/// <returns>char *</returns>
char *binary_at(binary_ctx *ctx, size_t pos);
/// <summary>
/// ��ȡint8
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <returns>int8_t</returns>
int8_t binary_get_int8(binary_ctx *ctx);
/// <summary>
/// ��ȡuint8
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <returns>uint8_t</returns>
uint8_t binary_get_uint8(binary_ctx *ctx);
/// <summary>
/// ��ȡ����ֵ
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="lens">�ֽ���</param>
/// <param name="islittle">1 С������ 0�������</param>
/// <returns>int64_t</returns>
int64_t binary_get_integer(binary_ctx *ctx, size_t lens, int32_t islittle);
/// <summary>
/// ��ȡ�޷�������ֵ
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="lens">�ֽ���</param>
/// <param name="islittle">1 С������ 0�������</param>
/// <returns>uint64_t</returns>
uint64_t binary_get_uinteger(binary_ctx *ctx, size_t lens, int32_t islittle);
/// <summary>
/// ��ȡfloatֵ
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="islittle">1 С������ 0�������</param>
/// <returns>float</returns>
float binary_get_float(binary_ctx *ctx, int32_t islittle);
/// <summary>
/// ��ȡdoubleֵ
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="islittle">1 С������ 0�������</param>
/// <returns>double</returns>
double binary_get_double(binary_ctx *ctx, int32_t islittle);
/// <summary>
/// ��ȡ�ַ���ֵ
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="lens">����,0 ȡ��'\0'����</param>
/// <returns>char *</returns>
char *binary_get_string(binary_ctx *ctx, size_t lens);
/// <summary>
/// ����ָ���ֽ�
/// </summary>
/// <param name="ctx">binary_ctx</param>
/// <param name="lens">����</param>
void binary_get_skip(binary_ctx *ctx, size_t lens);

#endif//BINARY_H_
