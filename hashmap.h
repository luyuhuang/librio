/**
 * @author: luyuhuang
 * @brief: Data structure: a hash map
 */

#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <stdio.h>
#include "list.h"

#define HASHMAP_INIT_CAPA       1024
#define HASHMAP_INIT_FACTOR     0.5f

/*return a hash code of key*/
typedef int (*hashmap_hs)(void*);
/*if arg1 == arg2, return 1(true) or return 0(false)*/
typedef int (*hashmap_eq)(void*, void*);

struct hashmap_pair {
    void *key;
    void *value;
};

struct hashmap {
    size_t len;
    size_t capacity;
    double resize_factor;

    hashmap_hs hs;
    hashmap_eq eq;

    list_t *lists;
};

struct hashmap_iter {
    size_t index;
    struct hashmap *map;
    struct list_node *p_node;
    size_t count;
};

typedef struct hashmap *hashmap_t;
typedef struct hashmap_iter *hashmap_iter_t;

hashmap_t hashmap_create(hashmap_hs hs, hashmap_eq eq);
hashmap_t hashmap_create_for_all(hashmap_hs hs, hashmap_eq eq, size_t init_capacity, double init_factor);
void hashmap_destroy(hashmap_t *map);

int hashmap_is_in(hashmap_t map, void *key);
int hashmap_add(hashmap_t map, void *key, void *value);
void *hashmap_get_value(hashmap_t map, void *key);
void *hashmap_del(hashmap_t map, void *key);
ssize_t hashmap_len(hashmap_t map);

hashmap_iter_t hashmap_iter_create(hashmap_t map);
void hashmap_iter_destroy(hashmap_iter_t *it);

struct hashmap_pair *hashmap_iter_next(hashmap_iter_t it);
int hashmap_iter_has_next(hashmap_iter_t it);

#endif //_HASHMAP_H_
