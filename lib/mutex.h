#ifndef MUTEX_H_
#define MUTEX_H_

#include "macro.h"

SREY_NS_BEGIN

class cmutex
{
public:
    cmutex();
    ~cmutex();

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

#ifdef OS_WIN
    CRITICAL_SECTION *getmutex()
#else
    pthread_mutex_t *getmutex()
#endif    
    {
        return &mutex;
    };

private:
#ifdef OS_WIN 
    CRITICAL_SECTION mutex;
#else
    pthread_mutex_t mutex;
#endif    
};

SREY_NS_END

#endif//MUTEX_H_
