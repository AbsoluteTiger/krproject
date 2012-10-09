#include "kr_skiplist.h"
#include "kr_alloc.h"
#include <time.h>


T_KRSkipListNode *
kr_skiplist_create_node(int level, unsigned int key, void *value) 
{
    T_KRSkipListNode *node = kr_malloc(sizeof(T_KRSkipListNode));
    if (node == NULL) {
        fprintf(stderr, "kr_malloc node failed!\n");
        return NULL;
    }
    
    node->key = key;
    node->value = value;
    node->forward = kr_calloc(level * sizeof(T_KRSkipListNode));
    if (node->forward == NULL) {
        fprintf(stderr, "kr_malloc node->forward failed!\n");
        kr_free(node);
        return NULL;
    }
    
    return node;
}

void kr_skiplist_free_node(T_KRSkipListNode *node) 
{
    kr_free(node->forward);
    kr_free(node);
}


T_KRSkipList *kr_skiplist_create(KRCompareFunc compare_func) 
{
    T_KRSkipList *krsl;
    int i;

    krsl = kr_malloc(sizeof(T_KRSkipList));
    if (krsl == NULL) {
        fprintf(stderr, "kr_malloc krsl failed!\n");
        return NULL;
    }
    krsl->header = kr_skiplist_create_node(ZSKIPLIST_MAXLEVEL, 0, NULL);
    for (i = 0; i < ZSKIPLIST_MAXLEVEL; i++) {
        krsl->header->forward[i] = NULL;
    }
    krsl->length = 0;
    krsl->level = 1;
    krsl->compare_func = compare_func;
    
    return krsl;
}


void kr_skiplist_free(T_KRSkipList *krsl) 
{
    T_KRSkipListNode *node = krsl->header->forward[0], *next;

    kr_free(krsl->header);
    while(node) {
        next = node->forward[0];
        kr_skiplist_free_node(node);
        node = next;
    }
    kr_free(krsl);
}


static int kr_skiplist_random_level(unsigned int key) 
{
    int level = 1;
    srandom((unsigned int)(time(NULL)+key));
    while (level<ZSKIPLIST_MAXLEVEL && (random()%ZSKIPLIST_BRANCHING) == 0) {
        level += 1;
    }
    return level;
}


static T_KRSkipListNode *
_kr_skiplist_lookup_internal(T_KRSkipList *krsl, T_KRSkipListNode **update, 
                            unsigned int key, void *value)
{
    T_KRSkipListNode *x;
    int i;
    
    x = krsl->header;
    for (i = krsl->level-1; i >= 0; i--) {
        while (x->forward[i] &&
              (x->forward[i]->key < key ||
                (x->forward[i]->key == key &&
                krsl->compare_func(x->forward[i]->value, value) < 0))) {
            x = x->forward[i];
        }
        update[i] = x;
    }
    
    return x->forward[0];
}


T_KRSkipListNode *
kr_skiplist_insert(T_KRSkipList *krsl, unsigned int key, void *value) 
{
    T_KRSkipListNode *update[ZSKIPLIST_MAXLEVEL], *x;
    int i, level;

    x = _kr_skiplist_lookup_internal(krsl, update, key, value);
    if (x && key == x->key && !krsl->compare_func(x->value, value)) {
        x->value = value;
        return x;
    }
    
    level = kr_skiplist_random_level(key);
    if (level > krsl->level) {
        for (i = krsl->level; i < level; i++) {
            update[i] = krsl->header;
        }
        krsl->level = level;
    }
    x = kr_skiplist_create_node(level, key, value);
    for (i = 0; i < level; i++) {
        x->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = x;
    }

    krsl->length++;
    return x;
}

int kr_skiplist_delete(T_KRSkipList *krsl, unsigned int key, void *value) 
{
    T_KRSkipListNode *update[ZSKIPLIST_MAXLEVEL], *x;
    int i;

    x = _kr_skiplist_lookup_internal(krsl, update, key, value);
    if (x && key == x->key && !krsl->compare_func(x->value, value)) {
        for (i = 0; i < krsl->level; i++) {
            if (update[i]->forward[i] == x) {
                update[i]->forward[i] = x->forward[i];
            }
        }
        
        kr_skiplist_free_node(x);
        
        while(krsl->level>1 && krsl->header->forward[krsl->level-1] == NULL)
            krsl->level--;
        
        krsl->length--;
        return 0;
    }
    
    return -1;
}


void *kr_skiplist_lookup(T_KRSkipList *krsl, unsigned int key, void *value)
{
    T_KRSkipListNode *update[ZSKIPLIST_MAXLEVEL], *x;

    x = _kr_skiplist_lookup_internal(krsl, update, key, value);
    if (x && key == x->key && !krsl->compare_func(x->value, value)) {
        return x->value;
    }
    
    return NULL;
}


void 
kr_skiplist_foreach(T_KRSkipList *krsl, KRForEachFunc foreach_func, void *data)
{
    T_KRSkipListNode *node = krsl->header->forward[0], *next;

    while(node) {
        next = node->forward[0];
        if (foreach_func) {
            foreach_func(node->value, data);
        }
        node = next;
    }
}


void *kr_skiplist_lookup_cycle(T_KRSkipList *krsl, unsigned int key)
{
    T_KRSkipListNode *x;
    int i;
    
    x = krsl->header;
    for (i = krsl->level-1; i >= 0; i--) {
        while (x->forward[i] && (x->forward[i]->key < key )) {
            x = x->forward[i];
        }
    }
    
    x = x->forward[0];
    if (x) {
        return x->value;
    } else if (krsl->header->forward[0]){
        return krsl->header->forward[0]->value;
    } else {
        return NULL;
    }
}