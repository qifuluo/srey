#ifndef XOR_H_
#define XOR_H_

#include "base/macro.h"

/// <summary>
/// ������
/// </summary>
/// <param name="key">��Կ</param>
/// <param name="round">����</param>
/// <param name="data">Ҫ���������</param>
/// <param name="lens">���ݳ���</param>
/// <returns>����������</returns>
void *xor_encode(const char key[4], const size_t round, void *data, const size_t lens);
/// <summary>
/// ������
/// </summary>
/// <param name="key">��Կ</param>
/// <param name="round">����</param>
/// <param name="data">Ҫ���������</param>
/// <param name="lens">���ݳ���</param>
/// <returns>����������</returns>
void *xor_decode(const char key[4], const size_t round, void *data, const size_t lens);

#endif//XOR_H_
