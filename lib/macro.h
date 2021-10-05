#ifndef MACRO_H_
#define MACRO_H_

#include "include.h"

#define MALLOC malloc
#define CALLOC calloc
#define REALLOC realloc
#define FREE free
#define ZERO(name, len) memset(name, 0, len)

#define SAFE_FREE(v_para)\
do\
{\
    if (NULL != v_para)\
    {\
        FREE(v_para);\
        v_para = NULL;\
    }\
}while(false)
#define SAFE_DEL(v_para)\
do\
{\
    if (NULL != v_para)\
    {\
        delete v_para;\
        v_para = NULL;\
    }\
}while(false)
#define SAFE_DELARR(v_para)\
do\
{\
    if (NULL != v_para)\
    {\
        delete[] v_para;\
        v_para = NULL;\
    }\
}while(false)

#define SREY_NS_BEGIN namespace srey {
#define SREY_NS_END }
#define SREY_NS srey

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
//s向上 取n(n 为2的倍数)的整数倍
#define ROUND_UP(s, n) (((s) + (n) - 1) & (~((n) - 1)))

#ifdef OS_WIN
    #define PATH_SEPARATOR '\\'
    #define SOCKET intptr_t
    #define PATH_LENS MAX_PATH
#else
    #define PATH_SEPARATOR '/'
    #define SIGNAL_EXIT SIGRTMIN + 10
    #define SOCKET int
    #define PATH_LENS PATH_MAX
#endif

#define H_CONCAT2(a, b) a b
#define H_CONCAT3(a, b, c) a b c
#define __FILENAME__ (strrchr(__FILE__, PATH_SEPARATOR) ? strrchr(__FILE__, PATH_SEPARATOR) + 1 : __FILE__)
#define PRINTF(fmt, ...) printf(H_CONCAT3("[%s %s %d] ", fmt, "\n"),  __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

//动态变量名
#define __ANONYMOUS(type, name, line)  type  name##line
#define _ANONYMOUS(type, line)  __ANONYMOUS(type, _anonymous, line)
#define ANONYMOUS(type)  _ANONYMOUS(type, __LINE__) 

#define ASSERTAB(Exp, pszMsg)\
do\
{\
    if (!(Exp))\
    {\
        PRINTF("%s", pszMsg);\
        abort();\
    }\
} while (false);

#define INIT_NUMBER 0
#define ONEK 1024
#define TIME_LENS 64
#define INVALID_SOCK -1

#ifdef OS_WIN
    #define IS_EAGAIN(e) (WSAEWOULDBLOCK == (e) || EAGAIN == (e))
    #define STRCMP _stricmp
    #define STRNCMP _strnicmp
    #define STRTOK strtok_s
    #define SNPRINTF _snprintf
    #define SWPRINTF swprintf
    #define STRNCPY strncpy_s
    #define ITOA _itoa_s
    #define STAT _stat
    #define USLEEP(us)\
    do\
    {\
        LARGE_INTEGER stft;\
        stft.QuadPart = -(10 * (__int64)us);\
        HANDLE htimer = CreateWaitableTimer(NULL, TRUE, NULL);\
        SetWaitableTimer(htimer, &stft, 0, NULL, NULL, 0);\
        WaitForSingleObject(htimer, INFINITE);\
        CloseHandle(htimer);\
    } while (false)
    #define MSLEEP(ms) Sleep(ms)
    #define TIMEB _timeb
    #define FTIME _ftime
    #define ACCESS _access
    #define SHUTDOWN(sock) shutdown(sock, SD_BOTH)
    #define CLOSESOCKET closesocket
    #define ATOMIC_ADD InterlockedExchangeAdd
    #define ATOMIC_SET InterlockedExchange
    //比较*ptr与oldval的值，如果两者相等，则将newval更新到*ptr并返回操作之前*ptr的值 成功 返回值等于oldval
    #define ATOMIC_CAS(ptr, oldval, newval) InterlockedCompareExchange(ptr, newval, oldval)
    #define LASTERROR() GetLastError()
    #define SOCKERROR() WSAGetLastError()
    #define ERRORSTR(errcode) 
#else
    #if EAGAIN == EWOULDBLOCK
        #define IS_EAGAIN(e) (EAGAIN == (e))
    #else
        #define IS_EAGAIN(e) (EAGAIN == (e) || EWOULDBLOCK == (e))
    #endif
    #define STRCMP strcasecmp
    #define STRNCMP strncasecmp
    #define STRTOK strtok_r
    #define SNPRINTF snprintf
    #define SWPRINTF swprintf
    #define STRNCPY strncpy
    #define ITOA itoa
    #define STAT stat
    #define USLEEP(us) usleep(us)
    #define MSLEEP(ms) usleep(ms * 1000)
    #define TIMEB timeb
    #define FTIME ftime
    #define ACCESS access
    #define SHUTDOWN(sock) shutdown(sock, SHUT_RDWR)
    #define CLOSESOCKET close
    #define ATOMIC_ADD __sync_fetch_and_add
    #define ATOMIC_SET __sync_lock_test_and_set
    //比较*ptr与oldval的值，如果两者相等，则将newval更新到*ptr并返回操作之前*ptr的值 成功 返回值等于oldval
    //type __sync_val_compare_and_swap (type *ptr, type oldval type newval); 
    #define ATOMIC_CAS(ptr, oldval, newval) __sync_val_compare_and_swap (ptr, oldval, newval)
    #define LASTERROR() (errno)
    #define SOCKERROR() (errno)
    #define ERRORSTR(errcode) strerror(errcode)
#endif

#define ATOMIC_GET(ppsrc) ATOMIC_ADD(ppsrc, 0)
#define SAFE_CLOSESOCK(fd)\
do\
{\
    if (INVALID_SOCK != fd)\
    {\
        CLOSESOCKET(fd);\
        fd = INVALID_SOCK;\
    }\
}while(false)

#endif//MACRO_H_
