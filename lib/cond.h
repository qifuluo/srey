#ifndef COND_H_
#define COND_H_

#include "mutex.h"

SREY_NS_BEGIN

class ccond
{
public:
    ccond()
    {
#ifdef OS_WIN
        InitializeConditionVariable(&m_cond);
#else
        ASSERTAB(ERR_OK == pthread_cond_init(&m_cond, (const pthread_condattr_t*)NULL),
            ERRORSTR(ERRNO));
#endif
    };
    ~ccond()
    {
#ifndef OS_WIN
        (void)pthread_cond_destroy(&m_cond);
#endif
    };
    /*
    * \brief          �ȴ��ź���
    */
    void wait(cmutex *pmu)
    {
#ifdef OS_WIN
        ASSERTAB(SleepConditionVariableCS(&m_cond, pmu->getmutex(), INFINITE),
            ERRORSTR(ERRNO));
#else
        ASSERTAB(ERR_OK == pthread_cond_wait(&m_cond, pmu->getmutex()),
            ERRORSTR(ERRNO));
#endif
    };
    /*
    * \brief          �ȴ��ź���
    * \param uims     �ȴ�ʱ��  ����
    */
    void timedwait(cmutex *pmu, const uint32_t &uims)
    {
#ifdef OS_WIN
        BOOL brtn = SleepConditionVariableCS(&m_cond, pmu->getmutex(), (DWORD)uims);
        if (!brtn)
        {
            ASSERTAB(ERROR_TIMEOUT == ERRNO, ERRORSTR(ERRNO));
        }
#else
        long seconds = uims / 1000;
        long nanoseconds = (uims - seconds * 1000) * 1000000;
        struct timeval now;

        gettimeofday(&now, NULL);

        struct timespec timewait;
        timewait.tv_sec = now.tv_sec + seconds;
        timewait.tv_nsec = now.tv_usec * 1000 + nanoseconds;
        if (timewait.tv_nsec >= 1000000000)
        {
            timewait.tv_nsec -= 1000000000;
            timewait.tv_sec++;
        }

        int32_t irtn = pthread_cond_timedwait(&m_cond, pmu->getmutex(), &timewait);
        ASSERTAB((ERR_OK == irtn || ETIMEDOUT == irtn), ERRORSTR(ERRNO));
#endif
    };
    /*
    * \brief          �����ź�
    */
    void signal()
    {
#ifdef OS_WIN
        WakeConditionVariable(&m_cond);
#else
        ASSERTAB(ERR_OK == pthread_cond_signal(&m_cond), ERRORSTR(ERRNO));
#endif
    };
    /*
    * \brief          ���������ź�
    */
    void broadcast()
    {
#ifdef OS_WIN
        WakeAllConditionVariable(&m_cond);
#else
        ASSERTAB(ERR_OK == pthread_cond_broadcast(&m_cond), ERRORSTR(ERRNO));
#endif
    };

private:
#ifdef OS_WIN
    CONDITION_VARIABLE m_cond;
#else
    pthread_cond_t m_cond;
#endif
};

SREY_NS_END

#endif//COND_H_
