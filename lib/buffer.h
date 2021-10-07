#ifndef BUFFER_H_
#define BUFFER_H_

#include "macro.h"

SREY_NS_BEGIN

class cbuffer
{
public:
    cbuffer();
    ~cbuffer();
    bool add(void *data, size_t datlen);
    void drain() {};

private:
    //���������ʼλ��
    uint8_t *buffer;
    //buffer��ʼ��ַ
    uint8_t *orig_buffer;
    //buffer��ʼ��ַ�����ݴ�ŵ�ַ��ƫ��
    size_t misalign;
    //�ܹ�buffer�ĳ���
    size_t totallen;
    //���������ݳ���
    size_t off;
};

SREY_NS_END

#endif//BUFFER_H_
