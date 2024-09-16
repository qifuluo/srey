#ifndef UTILS_H_
#define UTILS_H_

#include "base/macro.h"

typedef void *(*chr_func)(const void *, int32_t, size_t);
typedef int32_t(*cmp_func)(const void *, const void *, size_t);
/// <summary>
/// ��ȡһ����ID
/// </summary>
/// <returns>ID</returns>
uint64_t createid(void);
/// <summary>
/// ��ǰ�߳�ID
/// </summary>
/// <returns>�߳�ID</returns>
uint64_t threadid(void);
/// <summary>
/// ��coredump socket����������
/// </summary>
void unlimit(void);
/// <summary>
/// �źŴ���
/// </summary>
/// <param name="cb">������</param>
/// <param name="data">����</param>
void sighandle(void(*cb)(int32_t, void *), void *data);
/// <summary>
/// cpu������
/// </summary>
/// <returns>������</returns>
uint32_t procscnt(void);
/// <summary>
/// �Ƿ�Ϊ�ļ�
/// </summary>
/// <param name="file">·��</param>
/// <returns>ERR_OK �ļ�</returns>
int32_t isfile(const char *file);
/// <summary>
/// �Ƿ�Ϊ�ļ���
/// </summary>
/// <param name="file">·��</param>
/// <returns>ERR_OK �ļ�</returns>
int32_t isdir(const char *path);
/// <summary>
/// �ļ���С
/// </summary>
/// <param name="file">·��</param>
/// <returns>�ļ���С, ERR_FAILED ʧ��</returns>
int64_t filesize(const char *file);
/// <summary>
/// ��ǰ��������·��
/// </summary>
/// <returns>·��</returns>
const char *procpath(void);
/// <summary>
/// timeofday
/// </summary>
/// <param name="tv">timeval</param>
void timeofday(struct timeval *tv);
/// <summary>
/// ��UTCʱ��
/// </summary>
/// <returns>��</returns>
int32_t timeoffset(void);
/// <summary>
/// ��ǰʱ���
/// </summary>
/// <returns>����</returns>
uint64_t nowms(void);
/// <summary>
/// ��ǰʱ���
/// </summary>
/// <returns>��</returns>
uint64_t nowsec(void);
/// <summary>
/// ��ʽ�����ʱ���
/// </summary>
/// <param name="sec">��</param>
/// <param name="fmt">��ʽ�� %Y-%m-%d %H:%M:%S</param>
/// <param name="time">ʱ���ַ���</param>
void sectostr(uint64_t sec, const char *fmt, char time[TIME_LENS]);
/// <summary>
/// ��ʽ�����ʱ���
/// </summary>
/// <param name="ms">����</param>
/// <param name="fmt">��ʽ�� %Y-%m-%d %H:%M:%S</param>
/// <param name="time">ʱ���ַ���</param>
void mstostr(uint64_t ms, const char *fmt, char time[TIME_LENS]);
/// <summary>
/// �ַ���תʱ���
/// </summary>
/// <param name="time">ʱ���ַ���</param>
/// <param name="fmt">��ʽ��</param>
/// <returns>ʱ���</returns>
uint64_t strtots(const char *time, const char *fmt);
/// <summary>
/// ���timespec
/// </summary>
/// <param name="timeout">struct timespec</param>
/// <param name="ms">����</param>
void fill_timespec(struct timespec *timeout, uint32_t ms);
/// <summary>
/// hash
/// </summary>
/// <param name="buf">Ҫ���������</param>
/// <param name="len">���ݳ���</param>
/// <returns>hash</returns>
uint64_t hash(const char *buf, size_t len);
/// <summary>
/// �����ַ��������ִ�Сд
/// </summary>
/// <param name="ptr">Դ�ַ�</param>
/// <param name="val">��Ҫ���ҵ��ַ�</param>
/// <param name="maxlen">�����������</param>
/// <returns>void * �ַ����ֵ�ָ��, NULL��</returns>
void *memichr(const void *ptr, int32_t val, size_t maxlen);
/// <summary>
/// �ڴ����
/// </summary>
/// <param name="ncs">0 ���ִ�Сд</param>
/// <param name="ptr">Դ�ַ�</param>
/// <param name="plens">Դ�ַ�����</param>
/// <param name="what">Ҫ���ҵ��ַ���</param>
/// <param name="wlen">what����</param>
/// <returns>void * �ַ����ֵ�ָ��, NULL��</returns>
void *memstr(int32_t ncs, const void *ptr, size_t plens, const void *what, size_t wlen);
/// <summary>
/// �������ֽ�
/// </summary>
/// <param name="ptr">Դ�ַ�</param>
/// <param name="plens">Դ�ַ�����</param>
/// <returns>void *, NULLȫΪ��</returns>
void *skipempty(const void *ptr, size_t plens);
/// <summary>
/// ת��д
/// </summary>
/// <param name="str">Դ�ַ�</param>
/// <returns>char *</returns>
char *strupper(char *str);
/// <summary>
/// תСд
/// </summary>
/// <param name="str">Դ�ַ�</param>
/// <returns>char *</returns>
char *strlower(char *str);
/// <summary>
/// ��ת
/// </summary>
/// <param name="str">Դ�ַ�</param>
/// <returns>char *</returns>
char* strreverse(char* str);
/// <summary>
/// ���[min, max]
/// </summary>
/// <param name="min">��С</param>
/// <param name="max">���</param>
/// <returns>ֵ</returns>
int32_t randrange(int32_t min, int32_t max);
/// <summary>
/// ����ַ���
/// </summary>
/// <param name="buf">buffer</param>
/// <param name="len">�������</param>
/// <returns>char *</returns>
char *randstr(char *buf, size_t len);
#define HEX_ENSIZE(s) (s * 2 + 1)
/// <summary>
/// ת16����
/// </summary>
/// <param name="buf">Ҫת������</param>
/// <param name="len">���ݳ���</param>
/// <param name="out">ת���������,����:HEX_ENSIZE</param>
/// <returns>char *</returns>
char *tohex(const void *buf, size_t len, char *out);
/// <summary>
/// ���
/// </summary>
/// <param name="ptr">Ҫ��ֵ�����</param>
/// <param name="plens">���ݳ���</param>
/// <param name="sep">��ֱ��</param>
/// <param name="seplens">��ֱ�ǳ���</param>
/// <param name="n">��ֺ�ĳ���</param>
/// <returns>buf_ctx *, ��Ҫfree</returns>
struct buf_ctx *split(const void *ptr, size_t plens, const void *sep, size_t seplens, size_t *n);
/// <summary>
/// ���
/// </summary>
/// <param name="fmt">��ʽ��</param>
/// <param name="args">���</param>
/// <returns>char * ��Ҫfree</returns>
char *_format_va(const char *fmt, va_list args);
/// <summary>
/// ���
/// </summary>
/// <param name="fmt">��ʽ��</param>
/// <param name="...">���</param>
/// <returns>char * ��Ҫfree</returns>
char *format_va(const char *fmt, ...);
/// <summary>
/// ��С���ж�
/// </summary>
/// <returns>1 С��, 0 ���</returns>
int32_t is_little(void);
/// <summary>
/// ����ת char*
/// </summary>
/// <param name="buf">buffer</param>
/// <param name="val">����</param>
/// <param name="size">�ֽ���</param>
/// <param name="islittle">�Ƿ�ΪС��</param>
void pack_integer(char *buf, uint64_t val, int32_t size, int32_t islittle);
/// <summary>
/// char* ת����
/// </summary>
/// <param name="buf">Ҫת����buffer</param>
/// <param name="size">�ֽ���</param>
/// <param name="islittle">�Ƿ�ΪС��</param>
/// <param name="issigned">�Ƿ��з���</param>
/// <returns>����</returns>
int64_t unpack_integer(const char *buf, int32_t size, int32_t islittle, int32_t issigned);
/// <summary>
/// floatת char*
/// </summary>
/// <param name="buf">buffer</param>
/// <param name="val">ֵ</param>
/// <param name="islittle">�Ƿ�ΪС��</param>
void pack_float(char *buf, float val, int32_t islittle);
/// <summary>
/// char* תfloat
/// </summary>
/// <param name="buf">Ҫת����buffer</param>
/// <param name="islittle">�Ƿ�ΪС��</param>
/// <returns>float</returns>
float unpack_float(const char *buf, int32_t islittle);
/// <summary>
/// doubleת char*
/// </summary>
/// <param name="buf">buffer</param>
/// <param name="val">ֵ</param>
/// <param name="islittle">�Ƿ�ΪС��</param>
void pack_double(char *buf, double val, int32_t islittle);
/// <summary>
/// char* תdouble
/// </summary>
/// <param name="buf">Ҫת����buffer</param>
/// <param name="islittle">�Ƿ�ΪС��</param>
/// <returns>double</returns>
double unpack_double(const char *buf, int32_t islittle);
#if !defined(OS_WIN) && !defined(OS_DARWIN) && !defined(OS_AIX)
uint64_t ntohll(uint64_t val);
uint64_t htonll(uint64_t val);
#endif

#endif//UTILS_H_
