#ifndef MAP_H_
#define MAP_H_

#include "rwlock.h"

/*
* \brief              �½�
* \param uielsize     �ڵ㳤��
* \param hash         hash����
* \param compare      �ȽϺ���
* \param pudata       �û�����
* \return             map_ctx
*/
struct map_ctx *map_new(size_t uielsize, uint64_t(*hash)(void *),
    int32_t(*compare)(void *a, void *b, void *pudata), void *pudata);
/*
* \brief              �ͷ�
*/
void map_free(struct map_ctx *pmap);
struct rwlock_ctx *map_rwlock(struct map_ctx *pmap);
/*
* \brief              �ڵ���
* \return             �ڵ���
*/
size_t _map_size(struct map_ctx *pmap);
size_t map_size(struct map_ctx *pmap);
/*
* \brief              ����
* \param pitem        �ڵ�
*/
void _map_set(struct map_ctx *pmap, void *pitem);
void map_set(struct map_ctx *pmap, void *pitem);
/*
* \brief              ��ȡ
* \param pkey         key
* \param pitem        �ڵ�
* \return             ERR_OK �����ݣ� ������
*/
int32_t _map_get(struct map_ctx *pmap, void *pkey, void *pitem);
int32_t map_get(struct map_ctx *pmap, void *pkey, void *pitem);
/*
* \brief              ɾ��
* \param pkey         key
* \param pitem        �ڵ� NULL �����ر�ɾ���Ľڵ�
* \return             ERR_OK �����ݣ� ������
*/
int32_t _map_remove(struct map_ctx *pmap, void *pkey, void *pitem);
int32_t map_remove(struct map_ctx *pmap, void *pkey, void *pitem);
/*
* \brief              ���ȫ������
*/
void _map_clear(struct map_ctx *pmap);
void map_clear(struct map_ctx *pmap);
/*
* \brief              ����
* \param iter         ����ִ�еĺ������������ط�ERR_OK ִֹͣ��
* \param pudata       �û�����
*/
void _map_iter(struct map_ctx *pmap, int32_t(*iter)(void *pitem, void *pudata), void *pudata);
void map_iter(struct map_ctx *pmap, int32_t(*iter)(void *pitem, void *pudata), void *pudata);

#endif//MAP_H_
