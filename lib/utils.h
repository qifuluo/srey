#ifndef UTILS_H_
#define UTILS_H_

#include "macro.h"
//��ȡһID
uint64_t createid();
//��ǰ�߳�Id
uint32_t threadid();
//��coredump socket����������
void unlimit();
//�źŴ���
void sighandle(void(*cb)(int32_t, void *), void *data);
//�Ƿ�Ϊ���
int32_t bigendian();
//cpu������
uint32_t procscnt();

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
uint64_t nowms();
//��ǰʱ���  ��
uint64_t nowsec();
//��ʽ�������ǰʱ��� ��   %Y-%m-%d %H:%M:%S 
void nowtime(const char *fmt, char time[TIME_LENS]);
//��ʽ�������ǰʱ��� ����  %Y-%m-%d %H:%M:%S 
void nowmtime(const char *fmt, char time[TIME_LENS]);

uint16_t crc16(const char *pval, const size_t ilen);
uint32_t crc32(const char *pval, const size_t ilen);
void md5(const char *pval, const size_t ilens, char md5str[33]);
void sha1(const char *pval, const size_t ilens, char md5str[20]);
//����
#define B64_ENSIZE(s)   (((s) + 2) / 3 * 4)
#define B64_DESIZE(s)   (((s)) / 4 * 3)
int32_t b64encode(const char *pval, const size_t ilens, char *pout);
int32_t b64decode(const char *pval, const size_t ilens, char *pout);
char *xorencode(const char ackey[4], const size_t uiround, char *pbuf, const size_t uilens);
char *xordecode(const char ackey[4], const size_t uiround, char *pbuf, const size_t uilens);
char *urlencode(const char *s, const size_t len, size_t *new_length);
int32_t urldecode(char *str, size_t len);
uint64_t hash(const char *pfirst, size_t uilen);
uint64_t fnv1a_hash(const char *pfirst, size_t uilen);

//ת��д
char *strupper(char *str);
//תСд
char *strlower(char *str);
//��ת
char* strreverse(char* str);
//���
int32_t randrange(int32_t min, int32_t max);
//����ַ���
char *randstr(char *buf, size_t len);
//ת16�����ַ��� ����Ϊ ilens * 3 + 1
char *tohex(const char *buf, size_t len, char *out);
//��� ����ֵ��Ҫfree
char *formatargs(const char *fmt, va_list args);
char *formatv(const char *fmt, ...);

#endif//UTILS_H_
