#ifndef MACRO_H_
#define MACRO_H_

#include "config.h"
#include "errcode.h"
#include "macro_unix.h"
#include "macro_win.h"
#include "memory.h"

#define ONEK                 1024
#define TIME_LENS            64
#define IP_LENS              64
#define PORT_LENS            8
#define INVALID_FD           -1
#define KEEPALIVE_TIME       30
#define KEEPALIVE_INTERVAL   2

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define EMPTYSTR(str) ((NULL == str) || (0 == strlen(str)))

//s���� ȡn(n Ϊ2�ı���)��������
#define ROUND_UP(s, n) (((s) + (n) - 1) & (~((n) - 1)))

#define CONCAT2(a, b) a b
#define CONCAT3(a, b, c) a b c
#define __FILENAME__(file) (strrchr(file, PATH_SEPARATOR) ? strrchr(file, PATH_SEPARATOR) + 1 : file)
#define PRINT(fmt, ...) printf(CONCAT3("[%s %s %d] ", fmt, "\n"),  __FILENAME__(__FILE__), __FUNCTION__, __LINE__, ##__VA_ARGS__)

#ifndef offsetof
#define offsetof(type, field) ((size_t)(&((type *)0)->field))
#endif
#define UPCAST(ptr, type, field) ((type *)(((char*)(ptr)) - offsetof(type, field)))

//��̬������
#define __ANONYMOUS(type, name, line)  type  name##line
#define _ANONYMOUS(type, line)  __ANONYMOUS(type, _anonymous, line)
#define ANONYMOUS(type)  _ANONYMOUS(type, __LINE__)

#define BIT_SET(status, flag) status |= flag
#define BIT_CHECK(status, flag) (status & flag)
#define BIT_REMOVE(status, flag) status = status & ~flag

#define ATOMIC_GET(ptr) ATOMIC_ADD(ptr, 0)
#define ATOMIC64_GET(ptr) ATOMIC64_ADD(ptr, 0)

#define ZERO(name, len) memset(name, 0, len)
#define MALLOC(ptr, size) *(void**)&(ptr) = _malloc(size)
#define CALLOC(ptr, count, size) *(void**)&(ptr) = _calloc(count, size)
#define REALLOC(ptr, oldptr, size) *(void**)&(ptr) = _realloc(oldptr, size)
#define FREE(ptr)\
do {\
    if (NULL != ptr) {\
        _free(ptr); \
        ptr = NULL; \
    }\
}while (0)

#define CLOSE_SOCK(fd)\
do {\
    if (INVALID_SOCK != (fd) && 0 != (fd)) {\
        SOCK_CLOSE(fd);\
        (fd) = INVALID_SOCK;\
    }\
}while(0)

typedef enum LOG_LEVEL {
    LOGLV_FATAL = 0,
    LOGLV_ERROR,
    LOGLV_WARN,
    LOGLV_INFO,
    LOGLV_DEBUG,
}LOG_LEVEL;
void slog(int32_t lv, const char *fmt, ...);
#define LOG(lv, fmt, ...) slog(lv, CONCAT2("[%s %s %d] ", fmt), __FILENAME__(__FILE__), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG(LOGLV_FATAL, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(LOGLV_ERROR, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG(LOGLV_WARN, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG(LOGLV_INFO, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(LOGLV_DEBUG, fmt, ##__VA_ARGS__)

#define ASSERTAB(exp, errstr)\
do {\
    if (!(exp)) {\
        LOG_FATAL("%s", errstr);\
        exit(ERR_FAILED);\
    }\
} while (0);

#endif//MACRO_H_
