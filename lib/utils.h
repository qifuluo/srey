#ifndef UTILS_H_
#define UTILS_H_

#include "macro.h"

//获取一ID
uint64_t createid();
//当前线程Id
uint64_t threadid();
//启coredump socket链接数限制
void unlimit();
//信号处理
void sighandle(void(*cb)(int32_t, void *), void *data);
//是否为大端
int32_t bigendian();
//cpu核心数
uint32_t procscnt();

//是否为文件
int32_t isfile(const char *file);
//是否为文件夹
int32_t isdir(const char *path);
//文件大小 ERR_FAILED 失败
int64_t filesize(const char *file);
//当前程序所在路径
int32_t procpath(char path[PATH_LENS]);

//timeofday
void timeofday(struct timeval *tv);
//当前时间戳  毫秒
uint64_t nowms();
//当前时间戳  秒
uint64_t nowsec();
//格式化输出当前时间戳 秒   %Y-%m-%d %H:%M:%S 
void nowtime(const char *fmt, char time[TIME_LENS]);
//格式化输出当前时间戳 毫秒  %Y-%m-%d %H:%M:%S 
void nowmtime(const char *fmt, char time[TIME_LENS]);
//crc 16 IBM
uint16_t crc16(const char *buf, const size_t len);
uint32_t crc32(const char *buf, const size_t len);
void md5(const char *buf, const size_t len, char md5str[33]);
void sha1(const char *buf, const size_t lens, char sha1str[20]);
char *xorencode(const char key[4], const size_t round, char *buf, const size_t len);
char *xordecode(const char key[4], const size_t round, char *buf, const size_t len);
//返回值 需要FREE
char *b64encode(const char *buf, const size_t len, size_t *new_len);
//返回值 需要FREE
char *b64decode(const char *buf, const size_t len, size_t *new_len);
//返回值 需要FREE
char *urlencode(const char *str, const size_t len, size_t *new_len);
int32_t urldecode(char *str, size_t len);
uint64_t hash(const char *buf, size_t len);
uint64_t fnv1a_hash(const char *buf, size_t len);

//转大写
char *strupper(char *str);
//转小写
char *strlower(char *str);
//反转
char* strreverse(char* str);
//随机
int32_t randrange(int32_t min, int32_t max);
//随机字符串
char *randstr(char *buf, size_t len);
//转16进制字符串 长度为 ilens * 3 + 1
char *tohex(const char *buf, size_t len, char *out, size_t outlen);
//变参 返回值需要free
char *formatargs(const char *fmt, va_list args);
char *formatv(const char *fmt, ...);

#endif//UTILS_H_
