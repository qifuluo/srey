#ifndef EVSSL_H_
#define EVSSL_H_

#include "base/macro.h"
#if WITH_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "base/structs.h"

#define FREE_SSL(ssl)\
    if (NULL != ssl){\
        SSL_free(ssl); \
        ssl = NULL; \
    }
typedef struct evssl_ctx evssl_ctx;
/// <summary>
/// ������ʼ��
/// </summary>
void evssl_init(void);
/// <summary>
/// ����ca cert key
/// </summary>
/// <param name="ca">ca�ļ�, NULL �� "" ������</param>
/// <param name="cert">cert�ļ�, NULL �� "" ������</param>
/// <param name="key">key�ļ�, NULL �� "" ������</param>
/// <param name="type">֤������ SSL_FILETYPE_PEM,SSL_FILETYPE_ASN1</param>
/// <returns>evssl_ctx</returns>
evssl_ctx *evssl_new(const char *ca, const char *cert, const char *key, int32_t type);
/// <summary>
/// ����p12֤��
/// </summary>
/// <param name="p12">p12�ļ�, NULL �� "" ������</param>
/// <param name="pwd">֤������</param>
/// <returns>evssl_ctx</returns>
evssl_ctx *evssl_p12_new(const char *p12, const char *pwd);
/// <summary>
/// ��ȡSSL_CTX,����SSL_CTX_set_options SSL_CTX_set_verify��
/// </summary>
/// <param name="evssl">evssl_ctx</param>
/// <returns>SSL_CTX</returns>
SSL_CTX *evssl_sslctx(evssl_ctx *evssl);
/// <summary>
/// �ͷ�evssl_ctx
/// </summary>
/// <param name="evssl">evssl_ctx</param>
void evssl_free(evssl_ctx *evssl);
/// <summary>
/// evssl_ctx�س�ʼ��
/// </summary>
void evssl_pool_init(void);
/// <summary>
/// evssl_ctx���ͷ�
/// </summary>
void evssl_pool_free(void);
/// <summary>
/// ע��evssl_ctx
/// </summary>
/// <param name="name">����</param>
/// <param name="evssl">evssl_ctx</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t evssl_register(name_t name, evssl_ctx *evssl);
/// <summary>
/// �������ƻ�ȡevssl_ctx
/// </summary>
/// <param name="name">����</param>
/// <returns>evssl_ctx  NULLʧ��</returns>
evssl_ctx *evssl_qury(name_t name);
/// <summary>
/// ����socketΪSSL����
/// </summary>
/// <param name="evssl">evssl_ctx</param>
/// <param name="fd">socket���</param>
/// <returns>SSL  NULLʧ��</returns>
SSL *evssl_setfd(evssl_ctx *evssl, SOCKET fd);
/// <summary>
/// ��ʼ���������
/// </summary>
/// <param name="ssl">SSL</param>
/// <returns>1 �ɹ� ERR_OK ��Ҫ�������� ERR_FAILED ʧ��</returns>
int32_t evssl_tryacpt(SSL *ssl);
/// <summary>
/// ��ʼ�ͻ�������
/// </summary>
/// <param name="ssl">SSL</param>
/// <returns>1 �ɹ� ERR_OK ��Ҫ�������� ERR_FAILED ʧ��</returns>
int32_t evssl_tryconn(SSL *ssl);
/// <summary>
/// ���ݶ�ȡ
/// </summary>
/// <param name="ssl">SSL</param>
/// <param name="buf">�������ݵ�buffer</param>
/// <param name="len">����</param>
/// <param name="readed">�������ֽ���</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t evssl_read(SSL *ssl, char *buf, size_t len, size_t *readed);
/// <summary>
/// ����д��
/// </summary>
/// <param name="ssl">SSL</param>
/// <param name="buf">Ҫд�������</param>
/// <param name="len">����</param>
/// <param name="sended">д����ֽ���</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t evssl_send(SSL *ssl, char *buf, size_t len, size_t *sended);
/// <summary>
/// shutdown
/// </summary>
/// <param name="ssl">SSL</param>
/// <param name="fd">socket���</param>
void evssl_shutdown(SSL *ssl, SOCKET fd);

#endif//WITH_SSL
#endif//EVSSL_H_
