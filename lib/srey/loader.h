#ifndef LOADER_H_
#define LOADER_H_

#include "srey/spub.h"

/// <summary>
/// ������ȳ�ʼ��
/// </summary>
/// <param name="nnet">�����߳���, 0 cpu������</param>
/// <param name="nworker">�����߳���, 0 cpu������</param>
/// <returns>loader_ctx</returns>
loader_ctx *loader_init(uint16_t nnet, uint16_t nworker);
/// <summary>
/// ��������ͷ�
/// </summary>
/// <param name="loader">loader_ctx</param>
void loader_free(loader_ctx *loader);

#endif//LOADER_H_
