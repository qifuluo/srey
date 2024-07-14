#ifndef CUSTOMIZE_H_
#define CUSTOMIZE_H_

#include "base/structs.h"
#include "utils/buffer.h"

//���
struct custz_pack_ctx *custz_unpack(buffer_ctx *buf, ud_cxt *ud, size_t *size, int32_t *status);
/// <summary>
/// ���
/// </summary>
/// <param name="data">����</param>
/// <param name="lens">���ݳ���</param>
/// <param name="size">���������ݳ���</param>
/// <returns>custz_pack_ctx</returns>
struct custz_pack_ctx *custz_pack(void *data, size_t lens, size_t *size);
/// <summary>
/// ��ȡ����
/// </summary>
/// <param name="pack">custz_pack_ctx</param>
/// <param name="lens">���ݳ���</param>
/// <returns>����</returns>
void *custz_data(struct custz_pack_ctx *pack, size_t *lens);

#endif//CUSTOMIZE_H_
