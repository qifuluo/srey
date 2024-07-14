#ifndef URLRAW_H_
#define URLRAW_H_

#include "base/macro.h"

#define URLEN_SIZE(s) (3 * (s) + 1)
/// <summary>
/// URL����
/// </summary>
/// <param name="data">Ҫ���������</param>
/// <param name="lens">���ݳ���</param>
/// <param name="out">����������, ����:URLEN_SIZE(lens)</param>
/// <returns>����������</returns>
char *url_encode(const char *data, const size_t lens, char *out);
/// <summary>
/// URL����
/// </summary>
/// <param name="data">Ҫ���������</param>
/// <param name="lens">���ݳ���</param>
/// <returns>�����ĳ���</returns>
size_t url_decode(char *data, size_t lens);

#endif//URLRAW_H_
