#include "buffer.h"

SREY_NS_BEGIN

//                           ����buffer
//|                           totallen                   |
//|--------------|-----------|---------------------------|
//|misalign(ƫ��)|off(������)|ʣ��ռ�                   |
//|orig_buffer   |buffer     |

cbuffer::cbuffer()
{
    buffer = NULL;
    orig_buffer = NULL;
    misalign = INIT_NUMBER;
    totallen = INIT_NUMBER;
    off = INIT_NUMBER;
}
cbuffer::~cbuffer()
{
    SAFE_FREE(orig_buffer);
}

bool cbuffer::add(void *data, size_t datlen)
{
    return true;
}

SREY_NS_END
