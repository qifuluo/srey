#include "snowflake.h"
#include "utils.h"

SREY_NS_BEGIN

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

csnowflake::csnowflake(const int32_t &icenterid, const int32_t &iworkid)
{
    ASSERTAB(icenterid <= MaxDatacenterId && icenterid >= 0 
        && iworkid <= MaxWorkerId && iworkid >= 0, "param error.");

    m_centerid = icenterid;
    m_workid = iworkid;
    m_sequence = INIT_NUMBER;
    m_lasttime = nowmsec();
}
uint64_t csnowflake::_untilnextms()
{
    uint64_t ulcur = nowmsec();
    while (ulcur <= m_lasttime)
    {
        ulcur = nowmsec();
    }

    return ulcur;
}
uint64_t csnowflake::id()
{
    uint64_t uicur = nowmsec();
    ASSERTAB(uicur >= m_lasttime, "time error.");
    if (uicur == m_lasttime)
    {
        m_sequence = (m_sequence + 1) & SequenceMask;
        if (INIT_NUMBER == m_sequence)
        {
            //��ǰ�����ڼ������ˣ���ȴ���һ��
            uicur = _untilnextms();
        }
    }
    else
    {
        m_sequence = INIT_NUMBER;
    }

    m_lasttime = uicur;

    return ((uint64_t)(uicur - Epoch) << TimestampLeftShift) |
        ((uint64_t)m_centerid << DatacenterIdShift) |
        ((uint64_t)m_workid << WorkerIdShift) |
        m_sequence;
}

SREY_NS_END
