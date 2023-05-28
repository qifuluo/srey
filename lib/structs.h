#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "macro.h"

/*
�û�����
timeout: data(task) session
net: type(unpack_type) data(task) session  http(status, headlens, lens)
*/
typedef struct ud_cxt {
    int32_t type;
    int32_t status;
    void *data;
    size_t lens;
    size_t headlens;
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
