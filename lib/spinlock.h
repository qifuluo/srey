#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#include "macro.h"

#if defined(OS_WIN)
typedef CRITICAL_SECTION spin_ctx;
#elif defined(OS_DARWIN)
typedef os_unfair_lock spin_ctx;
#else
typedef pthread_spinlock_t spin_ctx;
#endif
/*
* \brief          ��ʼ��  ONEK
*/
void spin_init(spin_ctx *pctx, const uint32_t uispcnt);
/*
* \brief          �ͷ�
*/
void spin_free(spin_ctx *pctx);
/*
* \brief          ����
*/
void spin_lock(spin_ctx *pctx);
/*
* \brief          ����������
* \return         ERR_OK �ɹ�
*/
int32_t spin_trylock(spin_ctx *pctx);
/*
* \brief          ����
*/
void spin_unlock(spin_ctx *pctx);

#endif//SPINLOCK_H_
