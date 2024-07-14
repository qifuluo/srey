#ifndef THREAD_H_
#define THREAD_H_

#include "base/macro.h"

#if defined(OS_WIN)
typedef HANDLE pthread_t;
#endif
/// <summary>
/// �����߳�
/// </summary>
/// <param name="cb">�ص�����</param>
/// <param name="udata">�û�����</param>
/// <returns>pthread_t</returns>
pthread_t thread_creat(void(*cb)(void*), void *udata);
/// <summary>
/// �ȴ��߳̽���
/// </summary>
/// <param name="th">pthread_t</param>
void thread_join(pthread_t th);

#endif//THREAD_H_
