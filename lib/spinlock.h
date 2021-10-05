#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#include "macro.h"

SREY_NS_BEGIN

class cspinlock
{
public:
    cspinlock();
    ~cspinlock();

    /*
    * \brief          ����
    */
    void lock();
    /*
    * \brief          ����������
    * \return         true �ɹ�
    */
    bool trylock();
    /*
    * \brief          ����
    */
    void unlock();

private:
#ifdef OS_WIN
    CRITICAL_SECTION spin;
#else
    pthread_spinlock_t spin;
#endif
};

SREY_NS_END

#endif//SPINLOCK_H_
