#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "macro.h"

/*
�û�����
*/
typedef struct ud_cxt {
    uint8_t pktype;//���ݰ�����
    uint8_t status;//���״̬
    uint8_t slice;
    void *data;//task_ctx
    void *extra;//���ݰ�
    uint64_t sess;
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

#define COPY_UD(dst, src)\
do {\
    if (NULL != (src)){\
        (dst) = *(src);\
    }else{\
        ZERO(&(dst), sizeof(ud_cxt));\
    }\
} while (0)

#endif//STRUCTS_H_
