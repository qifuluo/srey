#ifndef QUEUE_H_
#define QUEUE_H_

#include "macro.h"

SREY_NS_BEGIN

class cqueue
{
public:
    explicit cqueue(const int32_t &icapacity)
    {
        ASSERTAB((icapacity > 0 && icapacity <= INT_MAX / (int32_t)sizeof(void*)), "capacity too large");
        m_size = INIT_NUMBER;
        m_next = INIT_NUMBER;
        m_capacity = icapacity;
        m_data = new(std::nothrow) void*[m_capacity];
        ASSERTAB(NULL != m_data, ERRSTR_MEMORY);
    };
    ~cqueue()
    {
        SAFE_DELARR(m_data);
    };
    /*
    * \brief          �������
    * \param pval     ��Ҫ��ӵ�����
    * \return         true �ɹ�
    */
    bool push(void *pval)
    {
        if (m_size >= m_capacity)
        {
            return false;
        }

        int32_t pos = m_next + m_size;
        if (pos >= m_capacity)
        {
            pos -= m_capacity;
        }

        m_data[pos] = pval;
        m_size++;

        return true;
    };
    /*
    * \brief          ����һ����
    * \return         NULL ������
    * \return         ����
    */
    void *pop()
    {
        void *pval = NULL;

        if (m_size > INIT_NUMBER)
        {
            pval = m_data[m_next];
            m_next++;
            m_size--;
            if (m_next >= m_capacity)
            {
                m_next -= m_capacity;
            }
        }

        return pval;
    };
    /*
    * \brief          ��ȡ��һ������
    * \return         NULL ������
    * \return         ����
    */
    void *peek()
    {
        return m_size ? m_data[m_next] : NULL;
    };
    const int32_t &getsize()
    {
        return m_size;
    };
    const int32_t &getcap()
    {
        return m_capacity;
    };

private:
    int32_t m_size;
    int32_t m_next;
    int32_t m_capacity;
    void **m_data;
};

SREY_NS_END

#endif//QUEUE_H_
