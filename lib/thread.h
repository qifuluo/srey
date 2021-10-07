#ifndef THREAD_H_
#define THREAD_H_

#include "macro.h"

SREY_NS_BEGIN

class ctask
{
public:
    ctask(){};
    ~ctask() {};
    virtual void beforrun() {};
    virtual void run() {};
    virtual void afterrun() {};
};

#define THREAD_WAITRUN  0
#define THREAD_RUNING   1
#define THREAD_STOP     2

typedef void(*thread_cb)(void*);//�̻߳ص�����

class cthread
{
public:
    cthread();
    ~cthread() {};
    /*
    * \brief             ����һ�߳�,���ε���
    * \param ptask       ctask
    */
    void creat(ctask *ptask);
    /*
    * \brief             ����һ�߳�,���ε���
    * \param func        �ص�����
    * \param pparam      �ص���������
    */
    void creat(thread_cb func, void *pparam);
    /*
    * \brief          �ȴ��߳�����
    */
    void waitstart();
    /*
    * \brief          �ȴ��߳̽���
    */
    void join();
    /*
    * \brief          ��ȡ��ǰ״̬
    */
    uint32_t state();
    /*
    * \brief          ��ȡ�������������߳�id
    */
    void setid(const uint32_t &uiid)
    {
        ATOMIC_SET(&threadid, uiid);
    };
    uint32_t getid()
    {
        return ATOMIC_GET(&threadid);
    };
    uint32_t *getstart()
    {
        return &start;
    };
    ctask *gettask()
    {
        return task;
    };
    thread_cb getfunc()
    {
        return funccb;
    };
    void *getparam()
    {
        return param;
    };
private:
    uint32_t threadid;
    uint32_t start;
    
#ifdef OS_WIN
    HANDLE pthread;
#else
    pthread_t pthread;//�߳̾��
#endif
    ctask *task;
    void *param;
    thread_cb funccb;
};

SREY_NS_END

#endif//THREAD_H_
