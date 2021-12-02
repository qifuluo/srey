#ifndef SREY_H_
#define SREY_H_

#include "netev/netev.h"

#define MSG_TYPE_UNREG       0x01    //ע��  
#define MSG_TYPE_TIMEOUT     0x02    //��ʱ    itype   uisess
#define MSG_TYPE_ACCEPT      0x03    //itype   pmsg(struct sock_ctx *)
#define MSG_TYPE_CONNECT     0x04    //itype   uisess  pmsg(struct sock_ctx *) uisize(error)
#define MSG_TYPE_RECV        0x05    //itype   pmsg(struct sock_ctx *)  uisize
#define MSG_TYPE_RECVFROM    0x06    //itype   pmsg(struct udp_recv_msg *) uisize
#define MSG_TYPE_SEND        0x07    //itype   pmsg(struct sock_ctx *)  uisize
#define MSG_TYPE_CLOSE       0x08    //itype   pmsg(struct sock_ctx *)
#define MSG_TYPE_REQUEST     0x09    //itype   srcid uisess  pmsg uisize
#define MSG_TYPE_RESPONSE    0x0A    //itype   uisess  pmsg uisize

struct task_ctx;
typedef void(*module_run)(struct task_ctx *ptask, uint32_t itype, sid_t srcid, uint32_t uisess, void *pmsg, uint32_t uisize, void *pud);
typedef void *(*module_create)(struct task_ctx *ptask, void *pud);
typedef int32_t(*module_init)(struct task_ctx *ptask, void *pmd, void *pud);
typedef void(*module_release)(struct task_ctx *ptask, void *pmd, void *pud);
typedef void(*module_msg_release)(void *pmsg);//�������Ϣ�ͷ�
struct module_ctx
{
    uint32_t maxcnt;//ÿ�����ִ��������
    module_create create;//����
    module_init init;//��ʼ��
    module_release release;//�ͷ�
    module_run run;//��Ϣ����
    char name[NAME_LENS];//������
};
struct udp_recv_msg
{
    uint16_t port;
    struct sock_ctx *sock;
    char ip[IP_LENS];
};
/*
* \brief          ��ʼ��
* \param uiworker �����߳���  0 ������ * 2
* \param msgfree  �����ͨ����Ϣ�ͷź���
*/
struct srey_ctx *srey_new(uint32_t uiworker, module_msg_release msgfree);
/*
* \brief          �ͷ�
*/
void srey_free(struct srey_ctx *pctx);
/*
* \brief          ����
*/
void srey_loop(struct srey_ctx *pctx);
/*
* \brief          ����ע��
* \param pmodule  struct module_ctx
* \param pudata   �û�����
* \return         0 ʧ��
* \return         ����ID
*/
sid_t srey_register(struct srey_ctx *pctx, struct module_ctx *pmodule, void *pudata);
/*
* \brief          ����ID��ѯ
* \param pname    ������
* \return         0 ��
* \return         ����ID
*/
sid_t srey_queryid(struct srey_ctx *pctx, const char *pname);
/*
* \brief          ��������ID��ѯ
* \param pqu      struct queue_ctx  id
*/
void srey_allid(struct srey_ctx *pctx, struct queue_ctx *pqu);
/*
* \brief          ����ע��
* \param id       ����id
*/
void srey_unregister(struct srey_ctx *pctx, sid_t id);
/*
* \brief          ������(toid/pname)������Ϣ������Ϣ�޷���
* \param toid     ������Ϣ������ID
* \param pmsg     ��Ϣ
* \param uisz     ��Ϣ����
* \return         ERR_OK �ɹ�
* \return         ���� ʧ��
*/
int32_t srey_callid(struct srey_ctx *pctx, sid_t toid, void *pmsg, uint32_t uisz);
int32_t srey_callnam(struct srey_ctx *pctx, const char *pname, void *pmsg, uint32_t uisz);
/*
* \brief          ������(toid/pname)������Ϣ
* \param toid     ������Ϣ������ID
* \param srcid    �������Ϣ������ID
* \param uisess   session ��ʶ
* \param pmsg     ��Ϣ
* \param uisz     ��Ϣ����
* \return         ERR_OK �ɹ�
* \return         ���� ʧ��
*/
int32_t srey_reqid(struct srey_ctx *pctx, sid_t toid, 
    sid_t srcid, uint32_t uisess, void *pmsg, uint32_t uisz);
int32_t srey_reqnam(struct srey_ctx *pctx, const char *pname, 
    sid_t srcid, uint32_t uisess, void *pmsg, uint32_t uisz);
/*
* \brief          ����srey_req����
* \param toid     ������Ϣ������ID
* \param uisess   session ��ʶ
* \param pmsg     ��Ϣ
* \param uisz     ��Ϣ����
* \return         ERR_OK �ɹ�
* \return         ���� ʧ��
*/
int32_t srey_response(struct srey_ctx *pctx, sid_t toid, 
    uint32_t uisess, void *pmsg, uint32_t uisz);
/*
* \brief           ע�ᳬʱ
* \param ownerid   �󶨵�����ID
* \param uisess    session ��ʶ
* \param uitimeout ��ʱʱ��
*/
void srey_timeout(struct srey_ctx *pctx, sid_t ownerid, uint32_t uisess, uint32_t uitimeout);
/*
* \brief           ����
* \param ownerid   �󶨵�����ID
* \param phost     ip
* \param usport    port
* \return          NULL ʧ��
* \return          struct listener_ctx
*/
struct listener_ctx *srey_listener(struct srey_ctx *pctx, sid_t ownerid, 
    const char *phost, uint16_t usport);
/*
* \brief           ����
* \param ownerid   �󶨵�����ID
* \param uisess    session ��ʶ
* \param utimeout  ��ʱ
* \param phost     ip
* \param usport    port
* \return          NULL ʧ��
* \return          struct sock_ctx
*/
struct sock_ctx *srey_connecter(struct srey_ctx *pctx, sid_t ownerid, uint32_t uisess,
    uint32_t utimeout, const char *phost, uint16_t usport);
/*
* \brief           ����Զ����socket
* \param sock      socket
* \param itype     SOCK_STREAM  SOCK_DGRAM
* \param ifamily   AF_INET  AF_INET6
* \return          NULL ʧ��
* \return          struct sock_ctx
*/
struct sock_ctx *srey_addsock(struct srey_ctx *pctx, SOCKET sock, int32_t itype, int32_t ifamily);
/*
* \brief           ��ʼ��д
* \param ownerid   �󶨵�����ID
* \param psock     struct sock_ctx
* \param iwrite    �Ƿ���ҪMSG_TYPE_SEND��Ϣ  0 ��
* \return          ERR_OK �ɹ�
* \return          ���� ʧ��
*/
int32_t srey_enable(struct srey_ctx *pctx, sid_t ownerid, struct sock_ctx *psock, int32_t iwrite);
/*
* \brief           ��ȡһsession
* \return          session
*/
uint32_t task_new_session(struct task_ctx *ptask);
/*
* \brief           ����ID
* \return          ID
*/
sid_t task_id(struct task_ctx *ptask);
/*
* \brief           ������
* \return          ������
*/
const char *task_name(struct task_ctx *ptask);

#endif//SREY_H_
