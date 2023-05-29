#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "macro.h"

/*
�û�����
timeout: data(task_ctx) session
net: upktype(unpack_type) data(task_ctx) session  http(extra)
*/
typedef struct ud_cxt {
    int32_t upktype;
    void *data;
    void *extra;
    uint64_t session;
}ud_cxt;

#define COPY_UD(dst, src)\
do {\
    if (NULL != (src)){\
        (dst) = *(src);\
    }else{\
        ZERO(&(dst), sizeof(ud_cxt));\
    }\
} while (0)

#endif//STRUCTS_H_
