#ifndef BASE64_H_
#define BASE64_H_

#include "base/macro.h"

#define B64EN_SIZE(s)   ((((s) + 2) / 3 * 4) + 1)
#define B64DE_SIZE(s)   (((s) / 4 * 3) + 1)
/// <summary>
/// base64 ����
/// </summary>
/// <param name="data">Ҫ���������</param>
/// <param name="lens">���ݳ���</param>
/// <param name="out">����������,Ԥ������:B64EN_SIZE(lens)</param>
/// <returns>���������ݳ���</returns>
size_t bs64_encode(const void *data, const size_t lens, char *out);
/// <summary>
/// base64 ����
/// </summary>
/// <param name="data">Ҫ���������</param>
/// <param name="lens">���ݳ���</param>
/// <param name="out">����������,Ԥ������:B64DE_SIZE(lens)</param>
/// <returns>���������ݳ���</returns>
size_t bs64_decode(const char *data, const size_t lens, char *out);

#endif//BASE64_H_
