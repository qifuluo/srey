#ifndef DES_H_
#define DES_H_

#include "base/macro.h"

#define DES_BLOCK_SIZE 8
typedef struct des_ctx {
    int32_t des3;
    uint8_t output[DES_BLOCK_SIZE];
    uint8_t schedule[3 * 16 * 6];
}des_ctx;
/// <summary>
/// des ��ʼ��
/// </summary>
/// <param name="des">des_ctx</param>
/// <param name="key">����</param>
/// <param name="klens">���볤��, ����8 3 * 8 �����0</param>
/// <param name="des3">1 3des, 0 des</param>
/// <param name="encrypt">1 ���� 0 ����</param>
void des_init(des_ctx *des, const char *key, size_t klens, int32_t des3, int32_t encrypt);
/// <summary>
/// des�ӽ���
/// </summary>
/// <param name="des">aes_ctx</param>
/// <param name="data">���ӽ�������,����:DES_BLOCK_SIZE</param>
/// <returns>�ӽ��ܺ������,����:DES_BLOCK_SIZE</returns>
char *des_crypt(des_ctx *des, const void *data);

#endif//DES_H_
