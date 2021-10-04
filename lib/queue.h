#ifndef QUEUE_H_
#define QUEUE_H_

#include "macro.h"

SREY_NS_BEGIN

class cqueue
{
public:
    cqueue(const int32_t &icapacity);
    ~cqueue();

    /*
    * \brief          �������
    * \param pval     ��Ҫ��ӵ�����
    * \return         true �ɹ�
    */
    bool push(void *pval);
    /*
    * \brief          ����һ����
    * \return         NULL ������
    * \return         ����
    */
    void *pop();
    /*
    * \brief          ��ȡ��һ������
    * \return         NULL ������
    * \return         ����
    */
    void *peek();
    const int32_t &getsize()
    {
        return size;
    };
    const int32_t &getcap()
    {
        return capacity;
    };

private:
    int32_t size;
    int32_t next;
    int32_t capacity;
    void **data;
};

SREY_NS_END

#endif//QUEUE_H_
