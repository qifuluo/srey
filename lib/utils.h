#ifndef UTILS_H_
#define UTILS_H_

#include "sarray.h"

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
int32_t procpath(char path[PATH_LENS]);

//timeofday
void timeofday(struct timeval *tv);
//��ǰʱ���  ����
uint64_t nowms(void);
//��ǰʱ���  ��
uint64_t nowsec(void);
//��ʽ�������ǰʱ��� ��   %Y-%m-%d %H:%M:%S 
void nowtime(const char *fmt, char time[TIME_LENS]);
//��ʽ�������ǰʱ��� ����  %Y-%m-%d %H:%M:%S 
void nowmtime(const char *fmt, char time[TIME_LENS]);
void fill_timespec(struct timespec* timeout, uint32_t ms);
//crc 16 IBM
uint16_t crc16(const char *buf, const size_t len);
uint32_t crc32(const char *buf, const size_t len);
void md5(const char *buf, const size_t len, char md5str[33]);
void sha1(const char *buf, const size_t lens, char sha1str[20]);
char *xorencode(const char key[4], const size_t round, char *buf, const size_t len);
char *xordecode(const char key[4], const size_t round, char *buf, const size_t len);
//����ֵ ��ҪFREE
char *b64encode(const char *buf, const size_t len, size_t *new_len);
//����ֵ ��ҪFREE
char *b64decode(const char *buf, const size_t len, size_t *new_len);
//����ֵ ��ҪFREE
char *urlencode(const char *str, const size_t len, size_t *new_len);
int32_t urldecode(char *str, size_t len);
uint64_t hash(const char *buf, size_t len);
uint64_t fnv1a_hash(const char *buf, size_t len);

void *memichr(const void *ptr, int32_t val, size_t maxlen);
#ifndef OS_WIN
int32_t _memicmp(const void *ptr1, const void *ptr2, size_t lens);
#endif
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
//ת16�����ַ��� ����Ϊ ilens * 3 + 1
char *tohex(const char *buf, size_t len, char *out, size_t outlen);
//��� ����ֵ��Ҫfree
char *formatargs(const char *fmt, va_list args);
char *formatv(const char *fmt, ...);

#endif//UTILS_H_
