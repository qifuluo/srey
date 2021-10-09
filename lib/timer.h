#ifndef TIMER_H_
#define TIMER_H_

#include "macro.h"

SREY_NS_BEGIN

class ctimer
{
public:
    ctimer();
    ~ctimer() {};

    /*
    * \brief          ��ǰʱ��
    */
    uint64_t nanosec();
    /*
    * \brief          ��ʼ��ʱ
    */
    void start();
    /*
    * \brief          ������ʱ
    * \return         ��ʱ ΢��
    */
    uint64_t elapsed();

private:
#ifdef OS_WIN
    double interval;
#endif
    uint64_t starttick;
};

SREY_NS_END

#endif//TIMER_H_
