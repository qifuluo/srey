#ifndef RWLOCK_H_
#define RWLOCK_H_

#include "base/macro.h"

typedef struct rwlock_ctx {
#if defined(OS_WIN)
    uint32_t wlock;
    SRWLOCK rwlock;
#else
    pthread_rwlock_t rwlock;
#endif
}rwlock_ctx;
/// <summary>
/// ��д����ʼ��
/// </summary>
/// <param name="ctx">rwlock_ctx</param>
static inline void rwlock_init(rwlock_ctx *ctx) {
#if defined(OS_WIN)
    InitializeSRWLock(&ctx->rwlock);
    ctx->wlock = 0;
#else
    ASSERTAB((ERR_OK == pthread_rwlock_init(&ctx->rwlock, NULL)),
        ERRORSTR(ERRNO));
#endif
};
/// <summary>
/// ��д���ͷ�
/// </summary>
/// <param name="ctx">rwlock_ctx</param>
static inline void rwlock_free(rwlock_ctx *ctx) {
#if defined(OS_WIN)
#else
    (void)pthread_rwlock_destroy(&ctx->rwlock);
#endif
};
/// <summary>
/// ������
/// </summary>
/// <param name="ctx">rwlock_ctx</param>
static inline void rwlock_rdlock(rwlock_ctx *ctx) {
#if defined(OS_WIN)
    AcquireSRWLockShared(&ctx->rwlock);
#else
    ASSERTAB(ERR_OK == pthread_rwlock_rdlock(&ctx->rwlock), ERRORSTR(ERRNO));
#endif
};
/// <summary>
/// ���Զ�����
/// </summary>
/// <param name="ctx">rwlock_ctx</param>
/// <returns>ERR_OK �ɹ�</returns>
static inline int32_t rwlock_tryrdlock(rwlock_ctx *ctx) {
#if defined(OS_WIN)
    return 0 != TryAcquireSRWLockShared(&ctx->rwlock) ? ERR_OK : ERR_FAILED;
#else
    return pthread_rwlock_tryrdlock(&ctx->rwlock);
#endif
};
/// <summary>
/// д����
/// </summary>
/// <param name="ctx">rwlock_ctx</param>
static inline void rwlock_wrlock(rwlock_ctx *ctx) {
#if defined(OS_WIN)
    AcquireSRWLockExclusive(&ctx->rwlock);
    ctx->wlock = 1;
#else
    ASSERTAB(ERR_OK == pthread_rwlock_wrlock(&ctx->rwlock), ERRORSTR(ERRNO));
#endif
};
/// <summary>
/// ����д����
/// </summary>
/// <param name="ctx">rwlock_ctx</param>
/// <returns>ERR_OK �ɹ�</returns>
static inline int32_t rwlock_trywrlock(rwlock_ctx *ctx) {
#if defined(OS_WIN)
    if (0 != TryAcquireSRWLockExclusive(&ctx->rwlock)) {
        ctx->wlock = 1;
        return ERR_OK;
    }
    return ERR_FAILED;
#else
    return pthread_rwlock_trywrlock(&ctx->rwlock);
#endif
};
/// <summary>
/// ����
/// </summary>
/// <param name="ctx">rwlock_ctx</param>
static inline void rwlock_unlock(rwlock_ctx *ctx) {
#if defined(OS_WIN)
    if (0 != ctx->wlock) {
        ctx->wlock = 0;
        ReleaseSRWLockExclusive(&ctx->rwlock);
    } else {
        ReleaseSRWLockShared(&ctx->rwlock);
    }
#else
    ASSERTAB(ERR_OK == pthread_rwlock_unlock(&ctx->rwlock), ERRORSTR(ERRNO));
#endif
};

#endif//RWLOCK_H_
