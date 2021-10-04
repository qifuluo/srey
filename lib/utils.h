#ifndef UTILS_H_
#define UTILS_H_

#include "macro.h"

SREY_NS_BEGIN
/*
* \brief          uint64_t�ֽ���ת��
* \param ulval    ��Ҫת����ֵ
* \return         ת�����ֵ
*/
uint64_t ntohl64(const uint64_t &ulval);
/*
* \brief          �߳�Id
* \return         �߳�Id
*/
uint32_t threadid();
/*
* \brief          cpu����
* \return         cpu����
*/
uint32_t procsnum();
/*
* \brief          ��ʽ���ַ���
* \param pformat  ��ʽ
* \param args     ���
* \return         ��ʽ������ַ���
*/
std::string formatv(const char *pformat, va_list args);
/*
* \brief          ��ʽ���ַ���
* \param pformat  ��ʽ
* \param ...      ���
* \return         ��ʽ������ַ���
*/
std::string formatstr(const char *pformat, ...);
/*
* \brief          �ж��ļ��Ƿ����
* \param pname    �ļ���
* \return         true ����
*/
bool fileexist(const char *pname);
/*
* \brief          �ж��Ƿ�Ϊ�ļ�
* \param pname    �ļ���
* \return         true ��
*/
bool isfile(const char *pname);
/*
* \brief          �ж��Ƿ�Ϊ�ļ���
* \param pname    �ļ���
* \return         true ��
*/
bool isdir(const char *pname);
/*
* \brief          �ļ���С
* \param pname    �ļ���
* \return         ERR_FAILED ʧ��
* \return         �ļ���С
*/
int64_t filesize(const char *pname);
/*
* \brief          �ļ�·��
* \param path     ȫ·��
* \return         ·��
*/
std::string dirnam(const char *path);
/*
* \brief          ��ȡ��ǰ��������·��
* \return         ""ʧ��
* \return         ·��
*/
std::string getpath();
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
* \brief          ��ȡsocket�ɶ�����
* \param fd       socket���
* \return         ERR_FAILED ʧ��
* \return         ����
*/
int32_t socknread(const SOCKET &fd);
/*
* \brief          ����crc16
* \param pval     ������
* \param ilen     pval����
* \return         crc16ֵ
*/
uint16_t crc16(const char *pval, const size_t &ilen);
/*
* \brief          ����crc32
* \param pval     ������
* \param ilen     pval����
* \return         crc32ֵ
*/
uint32_t crc32(const char *pval, const size_t &ilen);
/*
* \brief          siphash
* \param pin      ������
* \param inlen    pin����
* \param seed0    seed
* \param seed1    seed
* \return         siphashֵ
*/
uint64_t siphash64(const uint8_t *pin, const size_t &inlen, 
    const uint64_t &seed0, const uint64_t &seed1);
/*
* \brief          murmur hash3
* \param key      ������
* \param len      key����
* \param seed     seed
* \return         murmur hashֵ
*/
uint64_t murmurhash3(const void *key, const size_t &len, const uint32_t &seed);

SREY_NS_END

#endif//UTILS_H_
