#ifndef __KR_CONHASH_H__
#define __KR_CONHASH_H__

#include "kr_skiplist.h"
#include "kr_types.h"

/* forward declaretion */
typedef struct _kr_virtual_node_t T_KRVirtualNode;

typedef enum {
    KR_NODE_STATUS_CREATED = '0',
    KR_NODE_STATUS_INSERTED,
    KR_NODE_STATUS_DELETED
}E_KRNodeStatus;

/* actual node structure */
typedef struct _kr_actual_node_t
{
	unsigned int    hash;        /*node hash, hash(id)*/
	char            id[100];     /*node identifier, should be unique!*/
	unsigned int    weights;     /*node weights, the replicas of this node*/
	unsigned int    kicks;       /*node kicks, the kick times of this node*/
	T_KRVirtualNode *vnodes;     /*vitual nodes of this actual node*/
	void            *priv;       /*private data of this node*/
	E_KRNodeStatus  status;      /*node status*/
}T_KRActualNode;

/* virtual node structure */
struct _kr_virtual_node_t
{
	unsigned int    hash;        /* vnode hash, hash(vid)*/
	char            vid[128];    /* vnode identifier, come from nodeid!*/
	T_KRActualNode *node;        /* pointer to the actual node */
};

/* consistent hashing */
typedef struct _kr_conhash_t
{
	KRHashFunc      hash_func;   /* hash function */
	KRFreeFunc      free_func;   /* free function for private data */
	T_KRSkipList   *vnode_list;  /* skiplist of virtual nodes */
	T_KRSkipList   *node_list;   /* skiplist of actual nodes */
}T_KRConHash;



T_KRConHash *kr_conhash_construct(KRHashFunc hash_func, KRFreeFunc free_func);
void kr_conhash_destruct(T_KRConHash *krch);

T_KRActualNode *kr_conhash_lookup(T_KRConHash *krch, char *id);
int kr_conhash_add(T_KRConHash *krch, char *id, unsigned int weights, void *priv);
int kr_conhash_remove(T_KRConHash *krch, char *id);
int kr_conhash_adjust_weights(T_KRConHash *krch, char *id, unsigned int weights);

T_KRActualNode *kr_conhash_locate(T_KRConHash *krch, char *object);
void kr_conhash_foreach_node(T_KRConHash *krch, KRForEachFunc foreach_func, void *data);

#endif /* __KR_CONHASH_H__ */
