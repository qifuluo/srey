#ifndef COND_H_
#define COND_H_

#include "mutex.h"

SREY_NS_BEGIN

class ccond
{
public:
    ccond();
    ~ccond();
    /*
    * \brief          �ȴ��ź���
    */
    void wait(cmutex *pmu);
    /*
    * \brief          �ȴ��ź���
    * \param uims     �ȴ�ʱ��  ����
    */
    void timedwait(cmutex *pmu, const uint32_t &uims);
    /*
    * \brief          �����ź�
    */
    void signal();
    /*
    * \brief          ���������ź�
    */
    void broadcast();

private:
#ifdef OS_WIN
    CONDITION_VARIABLE cond;
#else
    pthread_cond_t cond;
#endif
};

SREY_NS_END

#endif//COND_H_
