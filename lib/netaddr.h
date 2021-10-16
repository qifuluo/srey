#ifndef NETADDR_H_
#define NETADDR_H_

#include "macro.h"

SREY_NS_BEGIN

class cnetaddr
{
public:
    cnetaddr();
    explicit cnetaddr(const bool &ipv6);
    ~cnetaddr() {};    
    /*
    * \brief          ���õ�ַ
    * \param phost    ip
    * \param usport   port
    * \param bipv6    �Ƿ�IP V6
    * \return         true �ɹ�
    * \return         ʧ��
    */
    bool setaddr(const char *phost, const uint16_t &usport);
    /*
    * \brief          ���õ�ַ
    * \param paddr    struct sockaddr *
    * \return         true �ɹ�
    * \return         ʧ��
    */
    bool setaddr(const struct sockaddr *paddr);
    /*
    * \brief          ��ȡԶ�˵�ַ��Ϣ
    * \param fd       SOCKET
    * \return         true �ɹ�
    * \return         ʧ��
    */
    bool setreaddr(const SOCKET &fd);
    /*
    * \brief          ��ȡ���ص�ַ��Ϣ
    * \param fd       SOCKET
    * \return         true �ɹ�
    * \return         ʧ��
    */
    bool setloaddr(const SOCKET &fd);
    /*
    * \brief          ���ص�ַ
    * \return         sockaddr *
    */
    sockaddr *getaddr();
    /*
    * \brief          ��ַ����
    * \return         ��ַ����
    */
    socklen_t getsize();
    /*
    * \brief          ��ȡIP
    * \return         ""ʧ��
    * \return         IP
    */
    std::string getip();
    /*
    * \brief          ��ȡ�˿�
    * \return         �˿�
    */
    uint16_t getport();
    /*
    * \brief          �Ƿ�Ϊipv4
    * \return         true ��
    */
    bool isipv4()
    {
        return IPV4 == m_type;
    };
    /*
    * \brief          �Ƿ�Ϊipv6
    * \return         true ��
    */
    bool isipv6()
    {
        return IPV6 == m_type;
    };
    int32_t addrfamily()
    {
        return IPV4 == m_type ? AF_INET : AF_INET6;
    };
    /*
    * \brief          �򵥼���Ƿ�Ϊipv4��δ�жϺϷ�
    * \return         true ��
    */
    static bool checkipv4(const char *phost);

private:
    void _clear();    

private:
    enum adrrtype
    {
        IPV4,
        IPV6
    };
    adrrtype m_type;
    sockaddr_in	m_ipv4;
    sockaddr_in6 m_ipv6;
};

SREY_NS_END

#endif//NETADDR_H_
