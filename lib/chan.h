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
    bool isclosed()
    {
        m_mmu.lock();
        bool bclosed = m_closed;
        m_mmu.unlock();

        return bclosed;
    };
    /*
    * \brief          д������
    * \param pdata    ��д�������
    * \return         true �ɹ�
    */
    bool send(void *pdata)
    {
        if (isclosed())
        {
            return false;
        }
        return NULL != m_queue ? _bufferedsend(pdata) : _unbufferedsend(pdata);
    };
    /*
    * \brief          ��ȡ����
    * \param pdata    ��ȡ��������
    * \return         true �ɹ�
    */
    bool recv(void **pdata)
    {
        return NULL != m_queue ? _bufferedrecv(pdata) : _unbufferedrecv(pdata);
    };
    /*
    * \brief          ��������
    * \return         ��������
    */
    int32_t size()
    {
        int32_t uisize = INIT_NUMBER;
        if (NULL != m_queue)
        {
            m_mmu.lock();
            uisize = m_queue->getsize();
            m_mmu.unlock();
        }
        return uisize;
    };
    /*
    * \brief          �Ƿ�ɶ�
    * \return         true ������
    */
    bool canrecv()
    {
        if (NULL != m_queue)
        {
            return size() > INIT_NUMBER;
        }

        m_mmu.lock();
        bool bcanrecv = m_wwaiting > INIT_NUMBER;
        m_mmu.unlock();

        return bcanrecv;
    };
    /*
    * \brief          �Ƿ��д
    * \return         true ��д
    */
    bool cansend()
    {
        bool bsend;
        if (NULL != m_queue)
        {
            m_mmu.lock();
            bsend = m_queue->getsize() < m_queue->getcap();
            m_mmu.unlock();
        }
        else
        {
            m_mmu.lock();
            bsend = m_rwaiting > INIT_NUMBER;
            m_mmu.unlock();
        }

        return bsend;
    };
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
    template <typename T>
    bool recvt(T **val)
    {
        return recv((void **)val);
    };

private:
    bool _bufferedsend(void *pdata);
    bool _bufferedrecv(void **pdata);
    bool _unbufferedsend(void* pdata);
    bool _unbufferedrecv(void **pdata);   

private:
    cmutex m_rmu;//����
    cmutex m_wmu;//д��    
    cmutex m_mmu;//��д�ź���
    ccond m_rcond;
    ccond m_wcond;
    bool m_closed;
    int32_t m_rwaiting;
    int32_t m_wwaiting;
    void *m_data;
    cqueue *m_queue;
};

SREY_NS_END

#endif//CHAN_H_
