#ifndef UTILS_H_
#define UTILS_H_

#include "macro.h"
/*
* \brief          ��ȡһID
*/
uint64_t createid();
/*
* \brief          ��ȡ��ǰ�߳�Id
* \return         �߳�Id
*/
uint64_t threadid();
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
void timeofday(struct timeval *ptv);
/*
* \brief          ��ȡ��ǰʱ���  ����
* \return         ʱ��  ����
*/
uint64_t nowmsec();
/*
* \brief          ��ȡ��ǰʱ���  ��
* \return         ʱ��  ��
*/
uint64_t nowsec();
/*
* \brief          ��ʽ�������ǰʱ��� ��
* \param pformat  ��ʽ   %Y-%m-%d %H:%M:%S 
* \param atime    ��ʽ���ʱ���ַ���  ��       
*/ 
void nowtime(const char *pformat, char atime[TIME_LENS]);
/*
* \brief          ��ʽ�������ǰʱ��� ����
* \param pformat  ��ʽ   %Y-%m-%d %H:%M:%S
* \param atime    ��ʽ���ʱ���ַ��� ����
*/
void nowmtime(const char *pformat, char atime[TIME_LENS]);
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

char *urlencode(const char *s, const size_t len, size_t *new_length);
int32_t urldecode(char *str, size_t len);
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
