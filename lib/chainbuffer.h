#ifndef BUFFER_H_
#define BUFFER_H_

#include "mutex.h"
#include "lockguard.h"

SREY_NS_BEGIN

struct consumecopy
{
    size_t offset;
    char *data;
};
bool _consumecopy(void *pudata, const char *pbuf, const size_t &uisize);

class cchainbuffer
{
public:
    cchainbuffer(const bool block = true);
    ~cchainbuffer();
    /*
    * \brief          �������
    * \param pdata    ����
    * \param uisize   ���ݳ���     
    */
    void produce(const void *pdata, const size_t &uisize);
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
    size_t size();
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

        clockguard<cmutex> lockthis(&mutex, lock);
        if (totallens >= sizeof(val))
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
    uint32_t _produce(const size_t &uisize, char *pcave[2], size_t casize[2], struct buffernode *nodes[2]);
    size_t _consume(const size_t &uisize, bool(*consumer)(void *, const char *, const size_t &),
        void *pudata, const bool &bdel = true);
    char *_search(struct buffernode *pnode, const char *pstart, const size_t &uissize,
        const char *pwhat, const size_t &uiwsize, const bool &bend);
    struct buffernode *_newnode(const size_t &uisize);
    struct buffernode *_newnode(const size_t &uiconsult, const size_t &uisize);
    void _freenode(struct buffernode *pnode);
    void _insert(struct buffernode *pnode);
    bool _should_realign(struct buffernode *pnode, const size_t &uisize);
    void _align(struct buffernode *pnode);    

private:
    bool lock;
    struct buffernode *head;
    struct buffernode *tail;
    size_t minisize;//512 1024
    size_t totallens;//�����ܳ���
    cmutex mutex;
};

SREY_NS_END

#endif//BUFFER_H_
