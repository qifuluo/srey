#ifndef TW_H_
#define TW_H_

#include "thread/thread.h"
#include "thread/spinlock.h"
#include "utils/timer.h"
#include "base/structs.h"

#define TVN_BITS (6)
#define TVR_BITS (8)
#define TVN_SIZE (1 << TVN_BITS)
#define TVR_SIZE (1 << TVR_BITS)
#define TVN_MASK (TVN_SIZE - 1)
#define TVR_MASK (TVR_SIZE - 1)
#define INDEX(N) ((ctx->jiffies >> (TVR_BITS + (N) * TVN_BITS)) & TVN_MASK)
typedef void(*tw_cb)(ud_cxt *ud);
typedef struct tw_node_ctx {
    struct tw_node_ctx *next;
    tw_cb _cb;//�ص�����
    free_cb _freecb;
    ud_cxt ud;//�û�����
    uint64_t expires;
}tw_node_ctx;
typedef struct tw_slot_ctx {
    tw_node_ctx *head;
    tw_node_ctx *tail;
}tw_slot_ctx;
typedef struct tw_ctx {
    volatile int32_t exit;
    uint64_t jiffies;
    spin_ctx spin;
    pthread_t thtw;
    timer_ctx timer;
    tw_slot_ctx reqadd;
    tw_slot_ctx tv1[TVR_SIZE];
    tw_slot_ctx tv2[TVN_SIZE];
    tw_slot_ctx tv3[TVN_SIZE];
    tw_slot_ctx tv4[TVN_SIZE];
    tw_slot_ctx tv5[TVN_SIZE];
}tw_ctx;
/// <summary>
/// ʱ���ֳ�ʼ��
/// </summary>
/// <param name="ctx">tw_ctx</param>
void tw_init(tw_ctx *ctx);
/// <summary>
/// ʱ�����ͷ�
/// </summary>
/// <param name="ctx">tw_ctx</param>
void tw_free(tw_ctx *ctx);
/// <summary>
/// ��Ӽ�ʱ����
/// </summary>
/// <param name="ctx">tw_ctx</param>
/// <param name="timeout">��ʱ ����</param>
/// <param name="_cb">��ʱ�ص�����</param>
/// <param name="_freecb">�����ͷź���</param>
/// <param name="ud">����</param>
void tw_add(tw_ctx *ctx, const uint32_t timeout, tw_cb _cb, free_cb _freecb, ud_cxt *ud);

#endif//TW_H_
