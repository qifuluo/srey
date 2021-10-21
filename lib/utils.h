#ifndef UTILS_H_
#define UTILS_H_

#include "macro.h"

/*
* \brief          uint64_t�ֽ���ת��
* \param ulval    ��Ҫת����ֵ
* \return         ת�����ֵ
*/
uint64_t ntohl64(const uint64_t ulval);
/*
* \brief          cpu����
* \return         cpu����
*/
uint16_t procsnum();
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
