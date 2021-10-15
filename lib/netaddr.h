#ifndef NETADDR_H_
#define NETADDR_H_

#include "macro.h"

SREY_NS_BEGIN

class cnetaddr
{
public:
    cnetaddr() {};
    ~cnetaddr() {};
    /*
    * \brief        ����sockaddr_in
    * \param phost  ip
    * \param port   port
    * \return       true �ɹ�
    */
    bool setaddr(const char *ip, const uint16_t &port);
    /*
    * \brief        ����sockaddr_in
    * \param paddr  sockaddr
    * \return       true �ɹ�
    */
    void setaddr(const struct sockaddr *paddr);
    /*
    * \brief        ����socket�����ȡԶ�˵�ַ��Ϣ
    * \param fd     SOCKET
    * \return       true �ɹ�
    */
    bool setremaddr(const SOCKET &fd);
    /*
    * \brief        ����socket�����ȡ���ص�ַ��Ϣ
    * \param fd     SOCKET
    * \return       true �ɹ�
    */
    bool setlocaddr(const SOCKET &fd);
    /*
    * \brief        ��ȡsockaddr
    * \return       sockaddr
    */
    sockaddr *getaddr();
    /*
    * \brief        ��ȡ��ַ����
    * \return       ����
    */
    size_t getsize();
    /*
    * \brief        ��ȡIP
    * \return       ip
    */
    std::string getip();
    /*
    * \brief        ��ȡ�˿�
    * \return       �˿�
    */
    uint16_t getport();

private:
    sockaddr_in	m_ipv4;
};

SREY_NS_END

#endif//NETADDR_H_
