#include "utils/popen2.h"
#include "utils/netutils.h"
#include "utils/utils.h"

#ifdef OS_WIN
#define PIPE_INBUF_SIZE  ONEK * 16
#define PIPE_OUTBUF_SIZE ONEK * 64
#define PIPE_PREFIX      "\\\\.\\pipe\\LOCAL\\srey_pipe_"

static int32_t _popen_pipe(HANDLE pipe[2]) {
    char pname[256];
    SNPRINTF(pname, sizeof(pname), "%s%"PRIu64, PIPE_PREFIX, createid());
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;//ʹ��ϵͳĬ�ϵİ�ȫ������ 
    sa.bInheritHandle = TRUE;//�����Ľ��̼̳о��
    HANDLE server = CreateNamedPipe(pname,//Ψһ�Ĺܵ�����
                                    PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,//��ģʽ
                                    PIPE_TYPE_BYTE,//�ܵ�ģʽ
                                    1,//��Ϊ�˹ܵ����������ʵ����
                                    PIPE_OUTBUF_SIZE,//����������������ֽ���
                                    PIPE_INBUF_SIZE,//���뻺�����������ֽ���
                                    0,//��ʱ Ϊ�㣬��Ĭ�ϳ�ʱΪ 50 ����
                                    &sa);
    if (INVALID_HANDLE_VALUE == server) {
        LOG_ERROR("%s", ERRORSTR(ERRNO));
        return ERR_FAILED;
    }
    HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL == event) {
        LOG_ERROR("%s", ERRORSTR(ERRNO));
        CloseHandle(server);
        return ERR_FAILED;
    }
    OVERLAPPED ovlpd;
    ZERO(&ovlpd, sizeof(OVERLAPPED));
    ovlpd.hEvent = event;
    if (!ConnectNamedPipe(server, &ovlpd)) {
        int32_t err = GetLastError();
        if (ERROR_IO_PENDING != err) {
            LOG_ERROR("%s", ERRORSTR(err));
            CloseHandle(event);
            CloseHandle(server);
            return ERR_FAILED;
        }
    }
    HANDLE client = CreateFile(pname,
                               GENERIC_READ | GENERIC_WRITE,
                               0,
                               NULL,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);
    if (INVALID_HANDLE_VALUE == client) {
        LOG_ERROR("%s", ERRORSTR(ERRNO));
        CloseHandle(event);
        CloseHandle(server);
        return ERR_FAILED;
    }
    if (WAIT_FAILED == WaitForSingleObject(event, INFINITE)) {
        CloseHandle(event);
        CloseHandle(server);
        CloseHandle(client);
        return ERR_FAILED;
    }
    CloseHandle(event);
    pipe[0] = server;
    pipe[1] = client;
    return ERR_OK;
}
#endif

int32_t popen_startup(popen_ctx *ctx, const char *cmd, const char *mode) {
    if (NULL == cmd
        || 0 == strlen(cmd)) {
        return ERR_FAILED;
    }
    int32_t r = 0, w = 0;
    if (NULL != mode) {
        r = NULL != strchr(mode, 'r');
        w = NULL != strchr(mode, 'w');
    }
    ZERO(ctx, sizeof(popen_ctx));
#ifdef OS_WIN
    if (r || w) {
        if (ERR_OK != _popen_pipe(ctx->pipe)) {
            return ERR_FAILED;
        }
    }
    STARTUPINFO startup;
    ZERO(&startup, sizeof(STARTUPINFO));
    startup.cb = sizeof(STARTUPINFO);
    startup.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    startup.wShowWindow = SW_HIDE;
    if (w) {
        startup.hStdInput = ctx->pipe[0];//�������ӹܵ�
    }
    if (r) {
        startup.hStdError = ctx->pipe[0];//������ӹܵ�
        startup.hStdOutput = ctx->pipe[0];
    }
    if (!CreateProcess(NULL,
                       TEXT((char *)cmd),
                       NULL,
                       NULL,
                       TRUE,
                       0,
                       NULL,
                       NULL,
                       &startup,
                       &ctx->process)) {
        LOG_ERROR("%s", ERRORSTR(ERRNO));
        popen_free(ctx);
        return ERR_FAILED;
    }
#else
    int sock[2];
    if (r || w) {
        if (ERR_FAILED == socketpair(AF_UNIX, SOCK_STREAM, 0, sock)) {
            LOG_ERROR("%s", ERRORSTR(ERRNO));
            return ERR_FAILED;
        }
    }
    pid_t pid = fork();
    if (0 == pid) {
        if (w) {
            dup2(sock[0], STDIN_FILENO);
        }
        if (r) {
            dup2(sock[0], STDOUT_FILENO);
            dup2(sock[0], STDERR_FILENO);
        }
        if (r || w) {
            close(sock[0]);
            close(sock[1]);
        }
        if (ERR_FAILED == execl("/bin/sh", "sh", "-c", cmd, NULL)){
            exit(ERRNO);
        }
    } else if (pid > 0) {
        ctx->pid = pid;
        if (r || w) {
            close(sock[0]);
            ctx->sock = sock[1];
        }
        return ERR_OK;
    } else {
        LOG_ERROR("%s", ERRORSTR(ERRNO));
        if (r || w) {
            close(sock[0]);
            close(sock[1]);
        }
        return ERR_FAILED;
    }
#endif
    return ERR_OK;
}
void popen_close(popen_ctx *ctx) {
#ifdef OS_WIN
    if (NULL == ctx->process.hProcess
        || 0 == ctx->process.dwProcessId) {
        return;
    }
    DWORD exitcode;
    if (!GetExitCodeProcess(ctx->process.hProcess, &exitcode)) {//����˳���
        LOG_ERROR("%s", ERRORSTR(ERRNO));
        return;
    }
    if (STILL_ACTIVE != exitcode) {//�Ƿ�������
        return;
    }
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��õ�ǰ���н��̵Ŀ���
    if (NULL == snapshot) {
        LOG_ERROR("%s", ERRORSTR(ERRNO));
        TerminateProcess(ctx->process.hProcess, ERR_FAILED);
        return;
    }
    HANDLE pvchild;
    PROCESSENTRY32 proentry32;
    proentry32.dwSize = sizeof(PROCESSENTRY32);
    BOOL ok = Process32First(snapshot, &proentry32);//��õ�һ�����̵ľ��
    while (ok) {
        if (proentry32.th32ParentProcessID == ctx->process.dwProcessId) {
            pvchild = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proentry32.th32ProcessID);
            if (NULL != pvchild) {
                TerminateProcess(pvchild, ERR_FAILED);
                CloseHandle(pvchild);
            } else {
                LOG_ERROR("%s", ERRORSTR(ERRNO));
            }
        }
        ok = Process32Next(snapshot, &proentry32);
    }
    TerminateProcess(ctx->process.hProcess, ERR_FAILED);
    CloseHandle(snapshot);
#else
    if (0 != ctx->pid
        && !ctx->exited) {
        kill(ctx->pid, SIGKILL);
        ctx->exited = 1;
        ctx->exitcode = ERR_FAILED;
    }
#endif
}
void popen_free(popen_ctx *ctx) {
#ifdef OS_WIN
    if (NULL != ctx->process.hProcess) {
        CloseHandle(ctx->process.hProcess);
    }
    if (NULL != ctx->process.hThread) {
        CloseHandle(ctx->process.hThread);
    }
    if (NULL != ctx->pipe[0]) {
        CloseHandle(ctx->pipe[0]);
    }
    if (NULL != ctx->pipe[1]) {
        CloseHandle(ctx->pipe[1]);
    }
#else
    if (0 != ctx->sock) {
        shutdown(ctx->sock, SHUT_RD);
        close(ctx->sock);
    }
#endif
}
#ifndef OS_WIN
static int32_t _child_exited(popen_ctx *ctx, int wstatus) {
    if (WIFEXITED(wstatus)) {//��������
        ctx->exited = 1;
        ctx->exitcode = WEXITSTATUS(wstatus);
        return ERR_OK;
    }
    if (WIFSIGNALED(wstatus)) {//�źŶ���ֹ
        ctx->exited = 1;
        ctx->exitcode = ERR_FAILED;
        return ERR_OK;
    }
#ifdef WCOREDUMP
    if (WCOREDUMP(wstatus)) {//core dump
        ctx->exited = 1;
        ctx->exitcode = ERR_FAILED;
        return ERR_OK;
    }
#endif
    return ERR_FAILED;
}
static int32_t _sock_closed(int32_t sock) {
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(sock, &rfd);
    struct timeval tv;
    tv.tv_usec = 1;
    tv.tv_sec = 0;
    int32_t rtn = select(sock + 1, &rfd, NULL, NULL, &tv);
    if (0 == rtn) {
        return 0;
    }
    if (rtn < 0){
        return 1;
    }
    rtn = sock_nread(sock);
    return rtn <= 0;
}
#endif
int32_t popen_waitexit(popen_ctx *ctx, uint32_t ms) {
#ifdef OS_WIN
    if (NULL == ctx->process.hProcess) {
        return ERR_OK;
    }
    if (WAIT_TIMEOUT == WaitForSingleObject(ctx->process.hProcess, (DWORD)ms)) {
        return ERR_FAILED;
    }
    return ERR_OK;
#else
    if (0 == ctx->pid
        || ctx->exited) {
        return ERR_OK;
    }
    if (_sock_closed(ctx->sock)) {
        ctx->exited = 1;
        ctx->exitcode = ERR_FAILED;
        return ERR_OK;
    }
    pid_t rtn;
    int wstatus;
    uint64_t startms = nowms();
    for (;;) {
        rtn = waitpid(ctx->pid, &wstatus, WNOHANG | WUNTRACED);
        if (ERR_FAILED == rtn) {
            LOG_ERROR("%s", ERRORSTR(ERRNO));
            return ERR_FAILED;
        }
        if (ctx->pid == rtn) {
            if (ERR_OK == _child_exited(ctx, wstatus)) {
                return ERR_OK;
            }
        }
        if (nowms() - startms >= ms) {
            return ERR_FAILED;
        }
        MSLEEP(1);
    }
#endif
}
int32_t popen_exitcode(popen_ctx *ctx) {
#ifdef OS_WIN
    if (NULL == ctx->process.hProcess) {
        return ERR_FAILED;
    }
    DWORD dwcode;
    if (!GetExitCodeProcess(ctx->process.hProcess, &dwcode)) {//����˳���
        LOG_ERROR("%s", ERRORSTR(ERRNO));
        return ERR_FAILED;
    }
    return (int32_t)dwcode;
#else
    if (0 == ctx->pid) {
        return ERR_FAILED;
    }
    if (ctx->exited) {
        return ctx->exitcode;
    }
    int wstatus = 0;
    if (ctx->pid == waitpid(ctx->pid, &wstatus, WNOHANG | WUNTRACED)) {
        if (ERR_OK == _child_exited(ctx, wstatus)) {
            return ctx->exitcode;
        }
    }
    return ERR_FAILED;
#endif
}
int32_t popen_read(popen_ctx *ctx, char *output, size_t lens) {
#ifdef OS_WIN
    if (NULL == ctx->pipe[1]) {
        return ERR_FAILED;
    }
    DWORD nread;
    if (!PeekNamedPipe(ctx->pipe[1], NULL, 0, NULL, &nread, NULL)) {
        return ERR_FAILED;
    }
    if (0 == nread) {
        return 0;
    }
    if (!ReadFile(ctx->pipe[1], output, (DWORD)lens, &nread, NULL)){
        return ERR_FAILED;
    }
    return (int32_t)nread;
#else
    if (0 == ctx->sock) {
        return ERR_FAILED;
    }
    int32_t nread = sock_nread(ctx->sock);
    if (ERR_FAILED == nread) {
        return ERR_FAILED;
    }
    if (0 == nread) {
        return 0;
    }
    nread = read(ctx->sock, output, lens);
    if (ERR_FAILED == nread) {
        return ERR_FAILED;
    }
    return nread;
#endif
}
int32_t popen_write(popen_ctx *ctx, const char *input, size_t lens) {
#ifdef OS_WIN
    if (NULL == ctx->pipe[1]) {
        return ERR_FAILED;
    }
    DWORD nwrite;
    if (!WriteFile(ctx->pipe[1], input, (DWORD)lens, &nwrite, NULL)) {
        return ERR_FAILED;
    }
    return (int32_t)nwrite;
#else
    if (0 == ctx->sock) {
        return ERR_FAILED;
    }
    return write(ctx->sock, input, lens);
#endif
}
