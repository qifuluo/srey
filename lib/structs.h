#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "macro.h"

typedef uint32_t name_t;
/*
�û�����
*/
typedef struct ud_cxt {
    uint8_t pktype;//���ݰ�����
    uint8_t status;//���״̬
    uint8_t client;//1 �ͻ���
    name_t name;//������
    void *data;//scheduler_ctx
    void *extra;//���ݰ�
    uint64_t sess;//timeout
}ud_cxt;
typedef struct buf_ctx {
    void *data;
    size_t lens;
}buf_ctx;
typedef struct off_buf_ctx {
    void *data;
    size_t len;
    size_t offset;
}off_buf_ctx;
struct task_ctx;

typedef void(*free_cb)(void *arg);
#define COPY_UD(dst, src)\
do {\
    if (NULL != (src)){\
        (dst) = *(src);\
    }else{\
        ZERO(&(dst), sizeof(ud_cxt));\
    }\
} while (0)

static inline int32_t buf_empty(buf_ctx *buf) {
    return NULL == buf || NULL == buf->data || 0 == buf->lens;
};
static inline int32_t buf_compare(buf_ctx *buf, const char *data, size_t lens) {
    return buf->lens == lens && 0 == memcmp(buf->data, data, lens);
};
static inline int32_t buf_icompare(buf_ctx *buf, const char *data, size_t lens) {
    return buf->lens == lens && 0 == _memicmp(buf->data, data, lens);
};

#endif//STRUCTS_H_
