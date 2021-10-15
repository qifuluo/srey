#ifndef BUFFER_H_
#define BUFFER_H_

#include "mutex.h"
#include "lockguard.h"

SREY_NS_BEGIN

struct buffernode
{
    struct buffernode *prior;
    struct buffernode *next;
    size_t bufferlens;
    size_t misalign;
    size_t offset;
    char *buffer;
};
struct consumecopy
{
    size_t offset;
    char *data;
};
bool _consumecopy(void *pudata, const char *pbuf, const size_t &uisize);

class cchainbuffer
{
public:
    explicit cchainbuffer(const bool block = true);
    ~cchainbuffer();
    /*
    * \brief          �������
    * \param pdata    ����
    * \param uisize   ���ݳ���
    */
    void produce(const void *pdata, const size_t &uisize);
    /*
    * \brief          �������
    * \param pfmt     ��ʽ
    * \param ...      ���
    * \return         ERR_FAILED ʧ��
    * \return         �ɹ���ӵ����ݳ���
    */
    int32_t producefmt(const char *pfmt, ...);
    /*
    * \brief            �������
    * \param uisize     ���ݳ���
    * \param filler     ������亯��,��pcave˳�����
    *                   ������(pudata, ����ַ����, ����ַ, ��ַ��Ӧ�����ݳ���)
    *                   ERR_FAILED ʧ�� ���������سɹ�����ֽ���
    * \param pudata     �û�����
    * \return           ERR_FAILED ʧ��
    * \return           �ɹ���ӵ����ݳ���
    */
    int32_t produce(const size_t &uisize,
        int32_t(*filler)(void *, const uint32_t &, char *pcave[2], size_t casize[2]),
        void *pudata);
    /*
    * \brief          �����ܳ���
    * \return         �����ܳ���
    */
    size_t size()
    {
        clockguard<cmutex> lockthis(&m_mutex, m_lock);
        return m_totallens;
    };
    /*
    * \brief          ����������
    * \param pdata    ���ݴ�ŵ�ַ
    * \param uisize   pdata����
    * \return         ʵ�ʿ��������ݳ���
    */
    size_t copy(void *pdata, const size_t &uisize);
    /*
    * \brief          ɾ������
    * \param uisize   ɾ������
    */
    size_t del(const size_t &uisize);
    /*
    * \brief          ������ɾ��buffer�е�����
    * \param pdata    ���ݴ�ŵ�ַ
    * \param uisize   pdata����
    * \return         ʵ�ʿ��������ݳ���
    */
    size_t remove(void *pdata, const size_t &uisize);
    /*
    * \brief          �������ݣ�ʹ�ú��Զ�ɾ��
    * \param uisize   ���ѳ���
    * \param consumer ���Ѻ��� ����:(�û�����, ���ѵ�����, ��������ʵ�ʳ���) ����false ����ֹ����ִ��
    * \param pudata   �û�����
    * \return         ʵ��ʹ�õ����ݳ���
    */
    size_t consume(const size_t &uisize, 
        bool (*consumer)(void *, const char *, const size_t &), void *pudata);
    /*
    * \brief          ��ȡһ����
    * \param  val     T
    * \return         true �ɹ�
    */
    template <typename T>
    bool gett(T &val)
    {
        consumecopy stccpy;
        stccpy.offset = INIT_NUMBER;
        stccpy.data = (char*)&val;

        clockguard<cmutex> lockthis(&m_mutex, m_lock);
        if (m_totallens >= sizeof(val))
        {
            ASSERTAB(sizeof(val) == _consume(sizeof(val), _consumecopy, &stccpy), "gett error.");
            return true;
        }

        return false;
    };
    /*
    * \brief          ����
    * \param uistart  ��ʼλ�ã���0��ʼ��
    * \param each     �ص�����,����ʧ�ܣ��˳����� ����(pudata, ����, ���ݳ���)
    * \param pudata   �û�����
    */
    void foreach(const size_t &uistart, bool(*each)(void *, const char *, const size_t &), void *pudata);
    /*
    * \brief          ���������ֽڱȽ�
    * \param uistart  ��ʼ������λ�ã���0��ʼ��
    * \param pdata    ����λ�ã���0��ʼ��  [ ]
    * \param pwhat    Ҫ����������
    * \param uiwsize  pwhat����
    * \return         ERR_FAILED δ�ҵ�
    * \return         ��һ�γ��ֵ�λ��
    */
    int32_t search(const size_t &uistart, const size_t &uiend, const char *pwhat, const size_t &uiwsize);
    /*
    * \brief          ��ӡ�����нڵ���Ϣ
    */
    void dump();

private:
    uint32_t _produce(const size_t &uisize, char *pcave[2], size_t casize[2], struct buffernode *nodes[2]); //Ԥ����   
    void _expand(const size_t &uisize);//��֤�����㹻���ڴ�    
    int32_t _producefmt(const char *pfmt, va_list args);
    size_t _consume(const size_t &uisize, bool(*consumer)(void *, const char *, const size_t &),
        void *pudata, const bool &bdel = true);
    char *_search(struct buffernode *pnode, const char *pstart, const size_t &uissize,
        const char *pwhat, const size_t &uiwsize, 
        const size_t &uitotaloff, const size_t &uiend, const bool &bend);
    bool _checkenogh(const size_t &uisize)
    {
        //Ϊ��
        if (NULL == m_tail)
        {
            _insert(_newnode(uisize));
            return true;
        }
        //ʣ���ֽ���
        if ((m_tail->bufferlens - m_tail->misalign - m_tail->offset) >= uisize)
        {
            return true;
        }
        //ͨ�������ܷ����
        if (_should_realign(m_tail, uisize))
        {
            _align(m_tail);
            return true;
        }

        return false;
    };
    struct buffernode *_newnode(const size_t &uisize)
    {
        size_t uitotal = ROUND_UP(uisize + sizeof(struct buffernode), m_minisize);
        char *pbuf = new(std::nothrow) char[uitotal];
        ASSERTAB(NULL != pbuf, ERRSTR_MEMORY);

        struct buffernode *pnode = (struct buffernode *)pbuf;
        ZERO(pnode, sizeof(struct buffernode));
        pnode->bufferlens = uitotal - sizeof(struct buffernode);
        pnode->buffer = (char *)(pnode + 1);

        return pnode;
    };
    struct buffernode *_newnode(const size_t &uiconsult, const size_t &uisize)
    {
        size_t uialloc = uiconsult;
        if (uialloc <= ONEK * 2)
        {
            uialloc <<= 1;
        }
        if (uisize > uialloc)
        {
            uialloc = uisize;
        }

        return _newnode(uialloc);
    };
    void _deltail()
    {
        if (m_head == m_tail)
        {
            return;
        }
        if (INIT_NUMBER != m_tail->offset)
        {
            return;
        }

        struct buffernode *ptmp = m_tail->prior;
        _freenode(m_tail);
        ptmp->next = NULL;
        m_tail = ptmp;
    };
    void _freenode(struct buffernode *pnode)
    {
        char *pbuf = (char*)pnode;
        SAFE_DELARR(pbuf);
    };
    void _insert(struct buffernode *pnode)
    {
        //��һ��
        if (NULL == m_tail)
        {
            m_head = m_tail = pnode;
            return;
        }

        pnode->prior = m_tail;
        m_tail->next = pnode;
        m_tail = pnode;
    };
    bool _should_realign(struct buffernode *pnode, const size_t &uisize)
    {
        return (pnode->bufferlens - pnode->offset >= uisize) &&
            (pnode->offset < pnode->bufferlens / 2) &&
            (pnode->offset <= ONEK *2);
    };
    void _align(struct buffernode *pnode)
    {
        memmove(pnode->buffer, pnode->buffer + pnode->misalign, pnode->offset);
        pnode->misalign = INIT_NUMBER;
    };

private:
    bool m_lock;
    struct buffernode *m_head;
    struct buffernode *m_tail;
    size_t m_minisize;//512 1024
    size_t m_totallens;//�����ܳ���
    cmutex m_mutex;
};

SREY_NS_END

#endif//BUFFER_H_
