#ifndef SFID_H_
#define SFID_H_

#include "base/macro.h"

typedef struct sfid_ctx {
    int32_t machinebitlen;//����idλ��
    int32_t sequencebitlen;//��������λ��
    int32_t timestampshift;
    int32_t machineidshift;
    int32_t sequence;//��������
    int32_t machineid;//����Id
    int32_t sequencemask;
    uint64_t customepoch;
    uint64_t lasttimestamp;
}sfid_ctx;
/// <summary>
/// snowflake id ��ʼ��
/// </summary>
/// <param name="ctx">sfid_ctx</param>
/// <param name="machineid">����ID</param>
/// <param name="machinebitlen">����IDλ��, 0 Ĭ��10</param>
/// <param name="sequencebitlen">��������λ��, 0 Ĭ��12</param>
/// <param name="customepoch">�̶�����, 0 Ĭ��</param>
/// <returns>NULLʧ��</returns>
sfid_ctx *sfid_init(sfid_ctx *ctx, int32_t machineid, int32_t machinebitlen, int32_t sequencebitlen, uint64_t customepoch);
/// <summary>
/// ��ȡID
/// </summary>
/// <param name="ctx">sfid_ctx</param>
/// <returns>snowflake id</returns>
uint64_t sfid_id(sfid_ctx *ctx);
/// <summary>
/// ͨ��ID������ ʱ��� ����ID ��������
/// </summary>
/// <param name="ctx">sfid_ctx</param>
/// <param name="id">snowflake id</param>
/// <param name="timestamp">ʱ��� ����</param>
/// <param name="machineid">����ID</param>
/// <param name="sequence">��������</param>
void sfid_decode(sfid_ctx *ctx, uint64_t id, uint64_t *timestamp, int32_t *machineid, int32_t *sequence);

#endif
