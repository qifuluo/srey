#ifndef RWLOCK_H_
#define RWLOCK_H_

#include "macro.h"

SREY_NS_BEGIN

class crwlock
{
public:
    crwlock();
    ~crwlock();

    /*
    * \brief          ������
    */
    void rdlock();
    /*
    * \brief          ������������
    * \return         true �ɹ�
    */
    bool tryrdlock();
    /*
    * \brief          д����
    * \return         true �ɹ�
    */
    void wrlock();
    /*
    * \brief          ������д����
    * \return         true �ɹ�
    */
    bool trywrlock();
    /*
    * \brief          ����
    */
    void unlock();

private:
#ifdef OS_WIN
    SRWLOCK lock;
    bool exclusive;
#else
    pthread_rwlock_t lock;
#endif
};

SREY_NS_END

#endif//RWLOCK_H_
