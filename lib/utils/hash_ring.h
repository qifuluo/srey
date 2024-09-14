#ifndef HASH_RING_H_
#define HASH_RING_H_

#include "crypt/digest.h"

typedef struct hash_ring_node {
    uint32_t nreplicas;//�ڵ���
    void *name;//�ڵ���
    size_t lens;//name ����
} hash_ring_node;
typedef struct hash_ring_ctx {
    struct hash_ring_list *nodes;
    uint32_t nnodes;
    struct hash_ring_item **items;
    uint32_t nitems;
    digest_ctx md5;
} hash_ring_ctx;

/// <summary>
/// ��ʼ��
/// </summary>
/// <param name="ring">hash_ring_ctx</param>
void hash_ring_init(hash_ring_ctx *ring);
/// <summary>
/// �ͷ�
/// </summary>
/// <param name="ring">hash_ring_ctx</param>
void hash_ring_free(hash_ring_ctx *ring);
/// <summary>
/// ��ӽڵ�
/// </summary>
/// <param name="ring">hash_ring_ctx</param>
/// <param name="name">�ڵ���</param>
/// <param name="lens">name����</param>
/// <param name="nreplicas">�ڵ���</param>
/// <returns>ERR_OK �ɹ� </returns>
int32_t hash_ring_add_node(hash_ring_ctx *ring, void *name, size_t lens, uint32_t nreplicas);
/// <summary>
/// ��ȡ�Ѿ���ӵĽڵ�
/// </summary>
/// <param name="ring">hash_ring_ctx</param>
/// <param name="name">�ڵ���</param>
/// <param name="lens">name����</param>
/// <returns>hash_ring_node</returns>
hash_ring_node *hash_ring_get_node(hash_ring_ctx *ring, void *name, size_t lens);
/// <summary>
/// �Ƴ��Ѿ���ӵĽڵ�
/// </summary>
/// <param name="ring">hash_ring_ctx</param>
/// <param name="name">�ڵ���</param>
/// <param name="lens">name����</param>
void hash_ring_remove_node(hash_ring_ctx *ring, void *name, size_t lens);
/// <summary>
/// ����key��Ӧ�Ľڵ�
/// </summary>
/// <param name="ring">hash_ring_ctx</param>
/// <param name="key">�ڵ���</param>
/// <param name="lens">key����</param>
/// <returns>hash_ring_node</returns>
hash_ring_node *hash_ring_find_node(hash_ring_ctx *ring, void *key, size_t lens);
/// <summary>
/// ��ӡ
/// </summary>
/// <param name="ring">hash_ring_ctx</param>
void hash_ring_print(hash_ring_ctx *ring);

#endif//HASH_RING_H_
