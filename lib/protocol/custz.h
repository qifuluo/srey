#ifndef CUSTOMIZE_H_
#define CUSTOMIZE_H_

#include "utils/buffer.h"

//���
void *custz_unpack(buffer_ctx *buf, size_t *size, int32_t *status);
/// <summary>
/// ���
/// </summary>
/// <param name="data">����</param>
/// <param name="lens">���ݳ���</param>
/// <param name="size">���������ݳ���</param>
/// <returns>void *</returns>
void *custz_pack(void *data, size_t lens, size_t *size);

#endif//CUSTOMIZE_H_
