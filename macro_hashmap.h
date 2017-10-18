/**
 * @author: luyuhuang
 * @brief: hash map using macro implementation
 */

#ifndef _MACRO_HASHMAP_H_
#define _MACRO_HASHMAP_H_

#include <stdio.h>
#include <stdlib.h>

#define HASHMAP(type, hash, eq, free)   \
    struct {                            \
        size_t len;                     \
        size_t capacity;                \
        double factor;                  \
        type **lists;                   \
        __typeof__(hash) hash;          \
        __typeof__(eq) eq;              \
        __typeof__(free) free;          \
    }

#define HASHMAP_INIT(map, capacity, factor) \
    do {                                                                    \
        map->len = 0;                                                       \
        map->capacity = capacity;                                           \
        map->factor = factor;                                               \
        map->lists = (__typeof__(map->lists))calloc(capacity, sizeof(*map->lists));   \
    } while (0)

#define HASHMAP_DEL(map) \
    do {                                                        \
        for (int i = 0; i < map->capacity; ++i) {               \
            __typeof__(*map->lists) t, p = map->lists[i];       \
            while (p) {                                         \
                t = p->__next__;                                \
                map->free(p);                                   \
                p = t;                                          \
            }                                                   \
        }                                                       \
        free(map->lists);
    } while (0)

#define HASHMAP_FOREACH(pair, map) \
        size_t _i_IN_HASHMAP_FOREACH = 0; \
        pair = map->lists[0];\
        while (true) {\
            bool _end_IN_HASHMAP_FOREACH = false;\
            while (pair == NULL) {\
                if (_i_IN_HASHMAP_FOREACH >= map->capacity) {\
                    _end_IN_HASHMAP_FOREACH = true; \
                    break;\
                }\
                pair = map->list[++_i_IN_HASHMAP_FOREACH];\
            }\
            if (_end_IN_HASHMAP_FOREACH) break;                                 


#define HASHMAP_FOREACH_END \
            pair = pair->__next__;                          \
        }                                                   

#define _hashmap_RESIZE(map) \
        do {
            size_t new_capacity = map->capacity * 2;
            __typeof__(map->lists) new_lists = 
                (__typeof__(map->lists))calloc(new_capacity, sizeof(*map->lists));
            __typeof__(*map->lists) itor;
            HASHMAP_FOREACH(itor, map)
                int hash_code = map->hash(pair->__key__);
                hash_code %= new_capacity;
                if (hash_code < 0) hash_code += new_capacity;
                itor->__next__ = new_lists[hash_code];
                new_lists[hash_code] = pair;
            HASHMAP_FOREACH_END
            free(map)
        } while (0)

#define HASHMAP_INSERT(map, pair) \
        do {
            if (map->len > map->capacity * map->factor) {
                _hashmap_RESIZE(map);
            }

            int hash_code = map->hash(pair->__key__);
            hash_code %= map->capacity;
            if (hash_code < 0) hash_code += map->capacity;
            __typeof__(*map->lists) p = _hashmap_LIST_FIND(map, map->lists[hash_code], pair->__key__);

            __typeof__(*map->lists) p = map->lists[hash_code], pp = NULL;
            bool end = false;
            while (p) {
                if (map->eq(p->__key__, pair->__key__)) {
                    pair->__next__ = p->__next__;
                    if (pp) pp->__next__ = pair;
                    else map->lists[hash_code] = pair;
                    map->free(p);
                    end = true;
                    break;
                }
                pp = p;
                p = p->__next__;
            }
            if (end) break;
            pair->__next__ = map->list[hash_code];
            map->list[hash_code] = pair;
            ++map->len;
        } while (0)

#endif //_MACRO_HASHMAP_H_
