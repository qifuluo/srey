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
* \brief          ��ȡ·���������ļ�
* \param ppath    ·��
* \param lstname  ���
* \param bdir     true �ļ���
* \return         �ļ�/�ļ���
*/
void filefind(const char *ppath ,std::list<std::string> &lstname, const bool bdir = false);
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
* \brief          �̶���ʽ�����ǰʱ��� ����
* \param atime    ��ǰʱ�� ����
*/
std::string nowmtime();
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
* \brief          ����md5
* \param pval     ������
* \param ilens    pval����
* \param md5str   md5ֵ
*/
void md5(const char *pval, const size_t &ilens, char md5str[33]);
/*
* \brief          ����sha1
* \param pval     ������
* \param ilens    pval����
* \param md5str   sha1ֵ
*/
void sha1(const char *pval, const size_t &ilens, char md5str[20]);

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
int32_t b64encode(const char *pval, const size_t &ilens, char *pout);
/*
* \brief          base64����
* \param pval     ��ת����
* \param ilens    pval����
* \return         ERR_FAILED ʧ�� 
* \return         ���볤��
*/
int32_t b64decode(const char *pval, const size_t &ilens, char *pout);
/*
* \brief          �ַ���ת��д
* \param pval     ��ת�����ַ���
* \return         ת��������ַ���
*/
char *toupper(char *pval);
/*
* \brief          �ַ���תСд
* \param pval     ��ת�����ַ���
* \return         ת��������ַ���
*/
char *tolower(char *pval);
/*
* \brief          ת16�����ַ���
* \param pval     ��ת����
* \param ilens    pval���� 
* \param bspace   �Ƿ��Կո�ֿ�
* \return         ת��������ַ���
*/
std::string tohex(const char *pval, const size_t &ilens, const bool bspace = true);
/*
* \brief          ��ʽ���ַ���
* \param pformat  ��ʽ
* \param args     ���
* \param iinit    ��ʼ��ʱ�ڴ��С
* \return         ��ʽ������ַ���,��Ҫdelete
*/
char *formatv(const char *pformat, va_list args, const size_t &iinit = 128);
/*
* \brief          ��ʽ���ַ���
* \param pformat  ��ʽ
* \param iinit    ��ʼ��ʱ�ڴ��С
* \param args     ���
* \return         ��ʽ������ַ���,��Ҫdelete
*/
char *formatv(const char *pformat, ...);
/*
* \brief          ��ʽ���ַ���
* \param pformat  ��ʽ
* \param ...      ���
* \return         ��ʽ������ַ���
*/
std::string formatstr(const char *pformat, ...);
/*
* \brief          �Ƴ���������ַ���
* \param str      �����ַ���
* \return         ����������
*/
std::string triml(const std::string &str);
/*
* \brief          �Ƴ��ұ������ַ���
* \param str      �����ַ���
* \return         ����������
*/
std::string trimr(const std::string &str);
/*
* \brief          �Ƴ����������ַ���
* \param str      �����ַ���
* \return         ����������
*/
std::string trim(const std::string &str);
/*
* \brief          ����ַ���
* \param str      �����ַ���
* \param pflag    ��ֱ�־
* \param empty    �Ƿ�������ַ���
* \return         ��ֺ�����ݣ����������ַ���
*/
void split(const std::string &str, const char *pflag, std::vector<std::string> &tokens, const bool empty = true);

SREY_NS_END

#endif//UTILS_H_
