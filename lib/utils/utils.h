#ifndef UTILS_H_
#define UTILS_H_

#include "base/macro.h"

typedef void *(*chr_func)(const void *, int32_t, size_t);
typedef int32_t(*cmp_func)(const void *, const void *, size_t);

//��ȡһID
uint64_t createid(void);
//��ǰ�߳�Id
uint64_t threadid(void);
//��coredump socket����������
void unlimit(void);
//�źŴ���
void sighandle(void(*cb)(int32_t, void *), void *data);
//cpu������
uint32_t procscnt(void);

//�Ƿ�Ϊ�ļ�
int32_t isfile(const char *file);
//�Ƿ�Ϊ�ļ���
int32_t isdir(const char *path);
//�ļ���С ERR_FAILED ʧ��
int64_t filesize(const char *file);
//��ǰ��������·��
const char *procpath(void);

//timeofday
void timeofday(struct timeval *tv);
//��UTCʱ�� ����
int32_t timeoffset(void);
//��ǰʱ���  ����
uint64_t nowms(void);
//��ǰʱ���  ��
uint64_t nowsec(void);
//��ʽ�����ʱ��� ��   %Y-%m-%d %H:%M:%S 
void sectostr(uint64_t sec, const char *fmt, char time[TIME_LENS]);
//��ʽ�����ʱ��� ����
void mstostr(uint64_t ms, const char *fmt, char time[TIME_LENS]);
//�ַ���תʱ���
uint64_t strtots(const char *time, const char *fmt);

void fill_timespec(struct timespec *timeout, uint32_t ms);

uint64_t hash(const char *buf, size_t len);

void *memichr(const void *ptr, int32_t val, size_t maxlen);
//�ڴ���� ncs 0 ���ִ�Сд
void *memstr(int32_t ncs, const void *ptr, size_t plens, const void *what, size_t wlen);
//�������ֽ�
void *skipempty(const void *ptr, size_t plens);
//ת��д
char *strupper(char *str);
//תСд
char *strlower(char *str);
//��ת
char* strreverse(char* str);
//���[min, max)
int32_t randrange(int32_t min, int32_t max);
//����ַ���
char *randstr(char *buf, size_t len);
//ת16�����ַ��� out ����Ϊ HEX_ENSIZE
#define HEX_ENSIZE(s) (s * 2 + 1)
char *tohex(const void *buf, size_t len, char *out);
//����ֵ��Ҫfree
struct buf_ctx *split(const void *ptr, size_t plens, const void *sep, size_t seplens, size_t *n);
//��� ����ֵ��Ҫfree
char *formatargs(const char *fmt, va_list args);
char *formatv(const char *fmt, ...);
int32_t is_little(void);
//���� char* ת��
void pack_integer(char *buf, uint64_t val, int32_t size, int32_t islittle);
int64_t unpack_integer(const char *buf, int32_t size, int32_t islittle, int32_t issigned);
void pack_float(char *buf, float val, int32_t islittle);
float unpack_float(const char *buf, int32_t islittle);
void pack_double(char *buf, double val, int32_t islittle);
double unpack_double(const char *buf, int32_t islittle);
#if !defined(OS_WIN) && !defined(OS_DARWIN) && !defined(OS_AIX)
uint64_t ntohll(uint64_t val);
uint64_t htonll(uint64_t val);
#endif

#endif//UTILS_H_