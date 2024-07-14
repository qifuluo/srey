#ifndef URL_PARSE_H_
#define URL_PARSE_H_

#include "base/structs.h"

#define MAX_NPARAM 16

typedef struct url_param {
    buf_ctx key;
    buf_ctx val;
}url_param;
typedef struct url_ctx {
    buf_ctx scheme;
    buf_ctx user;
    buf_ctx psw;
    buf_ctx host;
    buf_ctx port;
    buf_ctx path;
    buf_ctx anchor;
    url_param param[MAX_NPARAM];
}url_ctx;
/// <summary>
/// ��֧��Ƕ�� [Э������]://[������Դ��Ҫ��ƾ֤��Ϣ]@[��������ַ]:[�˿ں�]/[��Դ�㼶UNIX�ļ�·��][�ļ���]?[��ѯ]#[Ƭ��ID]
/// </summary>
/// <param name="ctx">url_ctx</param>
/// <param name="url">url</param>
/// <param name="lens">url����</param>
void url_parse(url_ctx *ctx, char *url, size_t lens);
/// <summary>
/// ��ȡ����
/// </summary>
/// <param name="ctx">url_ctx</param>
/// <param name="key">����</param>
/// <returns>buf_ctxֵ</returns>
buf_ctx *url_get_param(url_ctx *ctx, const char *key);

#endif//URL_PARSE_H_
