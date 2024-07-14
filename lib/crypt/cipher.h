#ifndef CIPHER_H_
#define CIPHER_H_

#include "crypt/aes.h"
#include "crypt/des.h"
#include "crypt/padding.h"

#define CIPHER_BLOCK_SIZE AES_BLOCK_SIZE
typedef char *(*_cipher_cb)(void *, const void *);
//�ӽ�������
typedef enum engine_type {
    DES = 0x01,
    DES3,
    AES
}engine_type;
//�ӽ���ģʽ
typedef enum cipher_model {
    ECB = 0x01,
    CBC,
    CFB,
    OFB,
    CTR
}cipher_model;
typedef struct cipher_ctx {
    int32_t encrypt;
    cipher_model model;
    padding_model padding;
    size_t block_lens;
    void *cur_ctx;
    _cipher_cb _cipher;
    uint8_t iv[CIPHER_BLOCK_SIZE];
    uint8_t cur_iv[CIPHER_BLOCK_SIZE];
    uint8_t pd_data[CIPHER_BLOCK_SIZE];
    uint8_t xor_data[CIPHER_BLOCK_SIZE];
    union {
        aes_ctx aes;
        des_ctx des;
    }eng_ctx;
}cipher_ctx;
/// <summary>
/// �ӽ��ܳ�ʼ��
/// </summary>
/// <param name="cipher">cipher_ctx</param>
/// <param name="engine">�ӽ�������</param>
/// <param name="model">�ӽ���ģʽ</param>
/// <param name="key">����</param>
/// <param name="klens">���볤��</param>
/// <param name="keybits">engineΪAES,ֵ 128 192 256</param>
/// <param name="encrypt">1 ���� 0 ����</param>
void cipher_init(cipher_ctx *cipher, engine_type engine, cipher_model model,
    const char *key, size_t klens, int32_t keybits, int32_t encrypt);
/// <summary>
/// ��ȡ�鳤��
/// </summary>
/// <param name="cipher">cipher_ctx</param>
/// <returns>�鳤��</returns>
size_t cipher_size(cipher_ctx *cipher);
/// <summary>
/// �������ģʽ.ECB CBC��Ҫ  CFB OFB CTR��ѡ
/// </summary>
/// <param name="cipher">cipher_ctx</param>
/// <param name="padding">���ģʽ</param>
void cipher_padding(cipher_ctx *cipher, padding_model padding);
/// <summary>
/// ����IV. CBC CFB OFB CTR��Ҫ
/// </summary>
/// <param name="cipher">cipher_ctx</param>
/// <param name="iv">IV</param>
/// <param name="ilens">IV����,С�ڿ鳤�Ȼ��Զ����</param>
void cipher_iv(cipher_ctx *cipher, const char *iv, size_t ilens);
/// <summary>
/// ����,׼����һ�ּӽ���
/// </summary>
/// <param name="cipher">cipher_ctx</param>
void cipher_reset(cipher_ctx *cipher);
/// <summary>
/// �ӽ���һ������
/// </summary>
/// <param name="cipher">cipher_ctx</param>
/// <param name="data">Ҫ�ӽ��ܵ�����</param>
/// <param name="lens">���ݳ���,С�ڵ��ڿ鳤��</param>
/// <param name="size">�ӽ��ܺ�ĳ���</param>
/// <returns>�ӽ��ܺ������</returns>
void *cipher_block(cipher_ctx *cipher, const void *data, size_t lens, size_t *size);
/// <summary>
/// �ӽ���һ������
/// </summary>
/// <param name="cipher">cipher_ctx</param>
/// <param name="data">Ҫ�ӽ��ܵ�����</param>
/// <param name="lens">���ݳ���</param>
/// <param name="output">�ӽ��ܺ������,����:data���� + �鳤��</param>
/// <returns>�ӽ��ܺ�ĳ���</returns>
size_t cipher_dofinal(cipher_ctx *cipher, const void *data, size_t lens, char *output);

#endif//CIPHER_H_
