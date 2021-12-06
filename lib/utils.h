#ifndef UTILS_H_
#define UTILS_H_

#include "macro.h"

/*
* \brief          ����coredump socket����������
*/
void unlimit();
/*
* \brief          �źŴ���
*/
void sighandle(void(*sig_cb)(int32_t, void *), void *pud);
/*
* \brief          �ж�ϵͳ�Ƿ�Ϊ���
* \param ulval    ��Ҫת����ֵ
* \return         ת�����ֵ
*/
int32_t bigendian();
/*
* \brief          cpu����
* \return         cpu����
*/
uint32_t procscnt();
/*
* \brief          �ж��Ƿ�Ϊ�ļ�
* \param pname    �ļ���
* \return         ERR_OK ��
*/
int32_t isfile(const char *pname);
/*
* \brief          �ж��Ƿ�Ϊ�ļ���
* \param pname    �ļ���
* \return         ERR_OK ��
*/
int32_t isdir(const char *pname);
/*
* \brief          �ļ���С
* \param pname    �ļ���
* \return         ERR_FAILED ʧ��
* \return         �ļ���С
*/
int64_t filesize(const char *pname);
/*
* \brief          ��ȡ��ǰ��������·��
* \param acpath   ·��
* \return         ERR_OK �ɹ�
*/
int32_t getprocpath(char acpath[PATH_LENS]);
/*
* \brief          ��ȡʱ��
* \param ptv      timeval
*/
static inline void timeofday(struct timeval *ptv)
{
#if defined(OS_WIN)
    #define U64_LITERAL(n) n##ui64
    #define EPOCH_BIAS U64_LITERAL(116444736000000000)
    #define UNITS_PER_SEC U64_LITERAL(10000000)
    #define USEC_PER_SEC U64_LITERAL(1000000)
    #define UNITS_PER_USEC U64_LITERAL(10)
    union
    {
        FILETIME ft_ft;
        uint64_t ft_64;
    } ft;

    GetSystemTimeAsFileTime(&ft.ft_ft);
    ft.ft_64 -= EPOCH_BIAS;
    ptv->tv_sec = (long)(ft.ft_64 / UNITS_PER_SEC);
    ptv->tv_usec = (long)((ft.ft_64 / UNITS_PER_USEC) % USEC_PER_SEC);
#else
    (void)gettimeofday(ptv, NULL);
#endif
};
/*
* \brief          ��ȡ��ǰʱ���  ����
* \return         ʱ��  ����
*/
static inline uint64_t nowmsec()
{
    struct timeval tv;
    timeofday(&tv);
    return (uint64_t)tv.tv_usec / 1000 + (uint64_t)tv.tv_sec * 1000;
};
/*
* \brief          ��ȡ��ǰʱ���  ��
* \return         ʱ��  ��
*/
static inline uint64_t nowsec()
{
    struct timeval tv;
    timeofday(&tv);
    return (uint64_t)tv.tv_sec;
};
/*
* \brief          ��ʽ�������ǰʱ��� ��
* \param pformat  ��ʽ   %Y-%m-%d %H:%M:%S 
* \param atime    ��ʽ���ʱ���ַ���  ��       
*/ 
static inline void nowtime(const char *pformat, char atime[TIME_LENS])
{
    struct timeval tv;
    timeofday(&tv);
    time_t t = tv.tv_sec;
    ZERO(atime, TIME_LENS);
    strftime(atime, TIME_LENS - 1, pformat, localtime(&t));
};
/*
* \brief          ��ʽ�������ǰʱ��� ����
* \param pformat  ��ʽ   %Y-%m-%d %H:%M:%S
* \param atime    ��ʽ���ʱ���ַ��� ����
*/
static inline void nowmtime(const char *pformat, char atime[TIME_LENS])
{
    struct timeval tv;
    timeofday(&tv);
    time_t t = tv.tv_sec;
    ZERO(atime, TIME_LENS);
    strftime(atime, TIME_LENS - 1, pformat, localtime(&t));
    size_t uilen = strlen(atime);
    SNPRINTF(atime + uilen, TIME_LENS - uilen - 1, " %03d", (int32_t)(tv.tv_usec / 1000));
};
/*
* \brief          ����crc16
* \param pval     ������
* \param ilen     pval����
* \return         crc16ֵ
*/
uint16_t crc16(const char *pval, const size_t ilen);
/*
* \brief          ����crc32
* \param pval     ������
* \param ilen     pval����
* \return         crc32ֵ
*/
uint32_t crc32(const char *pval, const size_t ilen);
/*
* \brief          ����md5
* \param pval     ������
* \param ilens    pval����
* \param md5str   md5ֵ
*/
void md5(const char *pval, const size_t ilens, char md5str[33]);
/*
* \brief          ����sha1
* \param pval     ������
* \param ilens    pval����
* \param md5str   sha1ֵ
*/
void sha1(const char *pval, const size_t ilens, char md5str[20]);

//����
#define B64_ENSIZE(s)   (((s) + 2) / 3 * 4)
#define B64_DESIZE(s)   (((s)) / 4 * 3)
/*
* \brief          תbase64
* \param pval     ��ת����
* \param ilens    pval����
* \return         ERR_FAILED ʧ��
* \return         ���볤��
*/
int32_t b64encode(const char *pval, const size_t ilens, char *pout);
/*
* \brief          base64����
* \param pval     ��ת����
* \param ilens    pval����
* \return         ERR_FAILED ʧ�� 
* \return         ���볤��
*/
int32_t b64decode(const char *pval, const size_t ilens, char *pout);
char *xorencode(const char ackey[4], const size_t uiround, char *pbuf, const size_t uilens);
char *xordecode(const char ackey[4], const size_t uiround, char *pbuf, const size_t uilens);
/*
* \brief          hash
* \return         hash
*/
uint64_t hash(const char *pfirst, size_t uilen);
uint64_t fnv1a_hash(const char *pfirst, size_t uilen);
/*
* \brief          �ַ���ת��д
* \param pval     ��ת�����ַ���
* \return         ת��������ַ���
*/
char *strtoupper(char *pval);
/*
* \brief          �ַ���תСд
* \param pval     ��ת�����ַ���
* \return         ת��������ַ���
*/
char *strtolower(char *pval);
/*
* \brief          ת16�����ַ���
* \param pval     ��ת����
* \param ilens    pval���� 
* \param          ת��������ַ���  ����Ϊ ilens * 3 + 1
*/
void tohex(const char *pval, const size_t ilens, char *pout);
/*
* \brief          ��ʽ���ַ���
* \param pformat  ��ʽ
* \param args     ���
* \param iinit    ��ʼ��ʱ�ڴ��С
* \return         ��ʽ������ַ���,��Ҫdelete
*/
char *formatargs(const char *pformat, va_list args);
/*
* \brief          ��ʽ���ַ���
* \param pformat  ��ʽ
* \param iinit    ��ʼ��ʱ�ڴ��С
* \param args     ���
* \return         ��ʽ������ַ���,��Ҫdelete
*/
char *formatv(const char *pformat, ...);

#endif//UTILS_H_
