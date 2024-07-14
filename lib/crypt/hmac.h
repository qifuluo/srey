#ifndef HMAC_H_
#define HMAC_H_

#include "crypt/digest.h"

typedef struct hmac_ctx {
    digest_ctx inside;
    digest_ctx outside;
    digest_ctx inside_init;
    digest_ctx outside_init;
}hmac_ctx;
/// <summary>
/// HMAC ��ʼ��
/// </summary>
/// <param name="hmac">hmac_ctx</param>
/// <param name="dtype">ժҪ�㷨</param>
/// <param name="key">����</param>
/// <param name="klens">���볤��</param>
void hmac_init(hmac_ctx *hmac, digest_type dtype, const char *key, size_t klens);
/// <summary>
/// ��ȡhash����
/// </summary>
/// <param name="hmac">hmac_ctx</param>
size_t hmac_size(hmac_ctx *hmac);
/// <summary>
/// ��������
/// </summary>
/// <param name="hmac">hmac_ctx</param>
/// <param name="data">����</param>
/// <param name="lens">���ݳ���</param>
void hmac_update(hmac_ctx *hmac, const void *data, size_t lens);
/// <summary>
/// ����hash
/// </summary>
/// <param name="hmac">hmac_ctx</param>
/// <param name="hash">hash, hash[DG_BLOCK_SIZE]</param>
/// <returns>hash����</returns>
size_t hmac_final(hmac_ctx *hmac, char *hash);
/// <summary>
/// ����,׼����һ�ּ���
/// </summary>
/// <param name="hmac">hmac_ctx</param>
void hmac_reset(hmac_ctx *hmac);

#endif//HMAC_H_
