#ifndef LOGER_H_
#define LOGER_H_

#include "singleton.h"
#include "utils.h"

SREY_NS_BEGIN

typedef enum LOG_LEVEL
{
    LOGLV_FATAL = 0,
    LOGLV_ERROR,
    LOGLV_WARN,
    LOGLV_INFO,
    LOGLV_DEBUG,
}LOG_LEVEL;

class cloger : public csingleton<cloger>
{
public:
    cloger();
    ~cloger();

    /*
    * \brief  ������־���𣬴��ڸü���ģ�����¼,֧�ֶ�̬����
    */
    void setlv(const LOG_LEVEL &emlv);
    /*
    * \brief  �����Ƿ��ӡ����Ļ,֧�ֶ�̬����
    */
    void setprint(const bool &bprint);
    /*
    * \brief          ��־
    * \param emlv     emlv ��־����
    * \param pformat  ��ʽ���ַ�
    */
    void log(const LOG_LEVEL &emlv, const char *pformat, ...);
    const char *getlvstr(const LOG_LEVEL &emlv);

private:
    uint32_t uilv;
    uint32_t uiprint;
    class cchan *pchan;
    class cthread *pthread;
    class clogertask *ptask;
};

#define SETLOGLV(lv) cloger::getinstance()->setlv(lv)
#define SETLOGPRT(bprt) cloger::getinstance()->setprint(bprt)

#define LOG(lv,format, ...)\
    (cloger::getinstance()->log(lv, CONCAT2("[%s][%s][%s %d]", format), \
    nowmtime().c_str(), cloger::getinstance()->getlvstr(lv), __FILENAME__, __LINE__, ##__VA_ARGS__))
#undef LOG_FATAL
#define LOG_FATAL(format, ...) LOG(LOGLV_FATAL, format, ##__VA_ARGS__)
#undef LOG_ERROR
#define LOG_ERROR(format, ...) LOG(LOGLV_ERROR, format, ##__VA_ARGS__)
#undef LOG_WARN
#define LOG_WARN(format, ...)  LOG(LOGLV_WARN, format, ##__VA_ARGS__)
#undef LOG_INFO
#define LOG_INFO(format, ...)  LOG(LOGLV_INFO, format, ##__VA_ARGS__)
#undef LOG_DEBUG
#define LOG_DEBUG(format, ...) LOG(LOGLV_DEBUG, format, ##__VA_ARGS__)

SREY_NS_END

#endif//LOGER_H_

