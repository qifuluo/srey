#ifndef LOG_H_
#define LOG_H_

#include "base/macro.h"

/// <summary>
/// ��־��ʼ��
/// </summary>
/// <param name="file">������</param>
void log_init(FILE *file);
/// <summary>
/// ������־����
/// </summary>
/// <param name="lv">LOG_LEVEL</param>
void log_setlv(LOG_LEVEL lv);

#endif//LOG_H_
