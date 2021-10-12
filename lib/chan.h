#ifndef CHAN_H_
#define CHAN_H_

#include "queue.h"
#include "mutex.h"
#include "cond.h"

SREY_NS_BEGIN

class cchan
{
public:
    /*
    * \param   icapacity ����0 �����������
    */
    cchan(const int32_t &icapacity);
    ~cchan();

    /*
    * \brief  �ر�channel���رպ���д��
    */
    void close();
    /*
    * \brief          channel�Ƿ�ر�
    * \return         true �Ѿ��ر�
    */
    bool isclosed();
    /*
    * \brief          д������
    * \param pdata    ��д�������
    * \return         true �ɹ�
    */
    bool send(void *pdata);
    /*
    * \brief          ��ȡ����
    * \param pdata    ��ȡ��������
    * \return         true �ɹ�
    */
    bool recv(void **pdata);
    /*
    * \brief          ��������
    * \return         ��������
    */
    int32_t size();
    /*
    * \brief          �Ƿ�ɶ�
    * \return         true ������
    */
    bool canrecv();
    /*
    * \brief          �Ƿ��д
    * \return         true ��д
    */
    bool cansend();
    /*
    * \brief             ���ѡȡһ�ɶ�д��channel��ִ�ж�д,�����Ĳ�֧��
    * \param precv_ctx   ��cchan
    * \param irecv_count ��cchan����
    * \param precv_out   ����������
    * \param psend_ctx   дcchan
    * \param isend_count дcchan����
    * \param psend_msgs  ÿ��cchan��Ҫ���͵�����
    * \return            ERR_FAILED ʧ��
    * \return            cchan �±�
    */
    static int32_t select(cchan *precv[], const int32_t &irecv_count, void **precv_out,
        cchan *psend[], const int32_t &isend_count, void *psend_msgs[]);

    template <typename T>
    bool sendt(const T &val)
    {
        T *pwrapped = new(std::nothrow) T();
        if (NULL == pwrapped)
        {
            return false;
        }
        *pwrapped = val;

        bool bsuccess = send(pwrapped);
        if (!bsuccess)
        {
            SAFE_DEL(pwrapped);
        }

        return bsuccess;
    };
    template <typename T>
    bool recvt(T &val)
    {
        void *pwrapped = NULL;
        bool bsuccess = recv(&pwrapped);
        if (NULL != pwrapped)
        {
            val = *((T*)pwrapped);
            delete((T *)pwrapped);
        }

        return bsuccess;
    };

private:
    bool _bufferedsend(void *pdata);
    bool _bufferedrecv(void **pdata);
    bool _unbufferedsend(void* pdata);
    bool _unbufferedrecv(void **pdata);   

private:
    cmutex r_mu;//����
    cmutex w_mu;//д��    
    cmutex m_mu;//��д�ź���
    ccond r_cond;
    ccond w_cond;
    bool closed;
    int32_t r_waiting;
    int32_t w_waiting;
    void *data;
    cqueue *queue;
};

SREY_NS_END

#endif//CHAN_H_
