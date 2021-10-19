#include "snowflake.h"
#include "utils.h"

#define Epoch 1358902800000LL
//������ʶλ��
#define WorkerIdBits 5
//�������ı�ʶλ��
#define DatacenterIdBits 5
//����ID���ֵ
#define MaxWorkerId  (-1 ^ (-1 << WorkerIdBits))
//��������ID���ֵ
#define MaxDatacenterId (-1 ^ (-1 << DatacenterIdBits))
//����������λ
#define SequenceBits 12
//����IDƫ����12λ
#define WorkerIdShift SequenceBits
//��������ID����17λ
#define DatacenterIdShift (SequenceBits + WorkerIdBits)
//ʱ���������22λ
#define TimestampLeftShift (SequenceBits + WorkerIdBits + DatacenterIdBits)
#define SequenceMask (-1 ^ (-1 << SequenceBits))

void sfid_init(struct sfid_ctx *pctx, const int32_t icenterid, const int32_t iworkid)
{
    ASSERTAB(icenterid <= MaxDatacenterId && icenterid >= 0 
        && iworkid <= MaxWorkerId && iworkid >= 0, "param error.");

    pctx->centerid = icenterid;
    pctx->workid = iworkid;
    pctx->sequence = 0;
    pctx->lasttime = nowmsec();
}
uint64_t _untilnextms(struct sfid_ctx *pctx)
{
    uint64_t ulcur = nowmsec();
    while (ulcur <= pctx->lasttime)
    {
        ulcur = nowmsec();
    }

    return ulcur;
}
uint64_t sfid_id(struct sfid_ctx *pctx)
{
    uint64_t uicur = nowmsec();
    ASSERTAB(uicur >= pctx->lasttime, "time error.");
    if (uicur == pctx->lasttime)
    {
        pctx->sequence = (pctx->sequence + 1) & SequenceMask;
        if (0 == pctx->sequence)
        {
            //��ǰ�����ڼ������ˣ���ȴ���һ��
            uicur = _untilnextms(pctx);
        }
    }
    else
    {
        pctx->sequence = 0;
    }

    pctx->lasttime = uicur;

    return ((uint64_t)(uicur - Epoch) << TimestampLeftShift) |
        ((uint64_t)pctx->centerid << DatacenterIdShift) |
        ((uint64_t)pctx->workid << WorkerIdShift) |
        pctx->sequence;
}
