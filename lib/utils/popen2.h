#ifndef POPEN2_H_
#define POPEN2_H_

#include "base/macro.h"

typedef struct popen_ctx {
#ifdef OS_WIN
    HANDLE pipe[2];
    PROCESS_INFORMATION process;
#else
    int32_t exited;
    int32_t exitcode;
    SOCKET sock;
    pid_t pid;
#endif
}popen_ctx;
/// <summary>
/// ִ������
/// </summary>
/// <param name="ctx">popen_ctx</param>
/// <param name="cmd">����</param>
/// <param name="mode">r�� wд</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t popen_startup(popen_ctx *ctx, const char *cmd, const char *mode);
/// <summary>
/// �رս���
/// </summary>
/// <param name="ctx">popen_ctx</param>
void popen_close(popen_ctx *ctx);
/// <summary>
/// �ͷ�
/// </summary>
/// <param name="ctx">popen_ctx</param>
void popen_free(popen_ctx *ctx);
/// <summary>
/// �ȴ�ִ�����
/// </summary>
/// <param name="ctx">popen_ctx</param>
/// <param name="ms">��ʱ ����</param>
/// <returns>ERR_OK �ɹ�</returns>
int32_t popen_waitexit(popen_ctx *ctx, uint32_t ms);
/// <summary>
/// ��ȡ�˳��� ��windows ��һ����ȡ��
/// </summary>
/// <param name="ctx">popen_ctx</param>
/// <returns>�˳���</returns>
int32_t popen_exitcode(popen_ctx *ctx);
/// <summary>
/// ��ȡ�������� r
/// </summary>
/// <param name="ctx">popen_ctx</param>
/// <param name="output">���</param>
/// <param name="lens">����</param>
/// <returns>�������ֽ���, ERR_FAILED ʧ��</returns>
int32_t popen_read(popen_ctx *ctx, char *output, size_t lens);
/// <summary>
/// ������д��,\n���� �Ż�ִ�� w
/// </summary>
/// <param name="ctx">popen_ctx</param>
/// <param name="input">����</param>
/// <param name="lens">����</param>
/// <returns>д����ֽ���, ERR_FAILED ʧ��</returns>
int32_t popen_write(popen_ctx *ctx, const char *input, size_t lens);

#endif//POPEN2_H_
