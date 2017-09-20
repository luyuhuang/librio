/**
 * @author: luyuhuang
 * @brief: Data structure: a hash map
 */

#include "hashmap.h"
#include <stdlib.h>
#include <assert.h>


hashmap_t hashmap_create(hashmap_hs hs, hashmap_eq eq)
{
    return hashmap_create_for_all(hs, eq, HASHMAP_INIT_CAPA, HASHMAP_INIT_FACTOR);
}

static bool _is_pair_equal(basic_value_t pair1, basic_value_t pair2)
{
    return BASIC2P(pair1, void*) == BASIC2P(pair2, void*);
}

hashmap_t hashmap_create_for_all(hashmap_hs hs, hashmap_eq eq, size_t init_capacity, double init_factor)
{
    if (!hs || !eq)
        return NULL;

    hashmap_t map = (struct hashmap*)calloc(1, sizeof(struct hashmap));

    map->len = 0;
    map->capacity = init_capacity;
    map->resize_factor = init_factor;
    map->hs = hs;
    map->eq = eq;

    map->lists = (list_t*)calloc(init_capacity, sizeof(list_t));
    for (int i = 0; i < init_capacity; i++) {
        map->lists[i] = list_create(_is_pair_equal);
    }
    return map;
}

void hashmap_destroy(hashmap_t *map)
{
    assert(map != NULL);
    assert((*map) != NULL);

    for (int i = 0; i < (*map)->capacity; i++) {
        list_iter_t it = list_iter_create((*map)->lists[i]);
        struct hashmap_pair *p;
        while ((p = BASIC2P(list_iter_next(it), struct hashmap_pair*)) != NULL) {
            free(p);
        }
        list_iter_destroy(&it);
        list_destroy((*map)->lists + i);
    }
    free((*map)->lists);
    free((*map));
    *map = NULL;
}

static struct hashmap_pair *_hashmap_list_find(hashmap_t map, list_t list, basic_value_t key)
{
    list_iter_t it = list_iter_create(list);
    struct hashmap_pair *p, *ret = NULL;
    while ((p = BASIC2P(list_iter_next(it), struct hashmap_pair*)) != NULL) {
        if (map->eq(p->key, key)) {
            ret = p;
            break;
        }
    }
    list_iter_destroy(&it);
    return ret;
}

static int _hashmap_resize(hashmap_t map)
{
    if (!map)
        return -1;

    size_t new_capacity = map->capacity * 2;
    list_t *new_lists = (list_t*)calloc(new_capacity, sizeof(list_t));
    for (int i = 0; i < new_capacity; i++) {
        new_lists[i] = list_create(_is_pair_equal);
    }
    hashmap_iter_t it = hashmap_iter_create(map);
    struct hashmap_pair *p;
    while ((p = hashmap_iter_next(it)) != NULL) {
        int hash_code = map->hs(p->key);
        hash_code %= new_capacity;
        if (hash_code < 0)
            hash_code += new_capacity;
        list_insert_at_tail(new_lists[hash_code], P2BASIC(p));
    }
    hashmap_iter_destroy(&it);
    for (int i = 0; i < map->capacity; i++)
        list_destroy(map->lists + i);
    free(map->lists);
    map->lists = new_lists;
    map->capacity = new_capacity;
    return 0;
}

int hashmap_add(hashmap_t map, basic_value_t key, basic_value_t value)
{
    if (!map)
        return -1;

    int hash_code = map->hs(key);
    hash_code %= map->capacity;
    if (hash_code < 0)
        hash_code += map->capacity;
    struct hashmap_pair *pair = _hashmap_list_find(map, map->lists[hash_code], key);
    if (pair == NULL) {
        pair = (struct hashmap_pair*)calloc(1, sizeof(struct hashmap_pair));
        assert(pair != NULL);
        pair->key = key;
        pair->value = value;
        list_insert_at_tail(map->lists[hash_code], P2BASIC(pair));
        map->len++;
        if (map->len > map->capacity * map->resize_factor) {
            _hashmap_resize(map);
        }
    } else {
        pair->value = value;
    }
    return 0;
}

basic_value_t hashmap_get_value(hashmap_t map, basic_value_t key)
{
    if (!map)
        return BASIC_NULL;

    int hash_code = map->hs(key);
    hash_code %= map->capacity;
    if (hash_code < 0)
        hash_code += map->capacity;
    struct hashmap_pair *pair = _hashmap_list_find(map, map->lists[hash_code], key);
    if (pair == NULL)
        return BASIC_NULL;
    else
        return pair->value;
}

int hashmap_is_in(hashmap_t map, basic_value_t key)
{
    if (!map)
        return -1;

    int hash_code = map->hs(key);
    hash_code %= map->capacity;
    if (hash_code < 0)
        hash_code += map->capacity;
    struct hashmap_pair *pair = _hashmap_list_find(map, map->lists[hash_code], key);
    return pair != NULL;
}

basic_value_t hashmap_del(hashmap_t map, basic_value_t key)
{
    if (!map)
        return BASIC_NULL;

    int hash_code = map->hs(key);
    hash_code %= map->capacity;
    if (hash_code < 0)
        hash_code += map->capacity;
    
    list_iter_t it = list_iter_create(map->lists[hash_code]);
    struct hashmap_pair *p;
    basic_value_t value = BASIC_NULL;
    int i = 0;
    while ((p = BASIC2P(list_iter_next(it), struct hashmap_pair*)) != NULL) {
        if (map->eq(p->key, key)) {
            value = p->value;
            list_del(map->lists[hash_code], i);
            free(p);
            map->len--;
            break;
        }
        i++;
    }
    list_iter_destroy(&it);
    return value;
}

ssize_t hashmap_len(hashmap_t map)
{
    if (!map)
        return -1;
    return map->len;
}


hashmap_iter_t hashmap_iter_create(hashmap_t map)
{
    if (!map)
        return NULL;

    hashmap_iter_t iter = (struct hashmap_iter*)calloc(1, sizeof(struct hashmap_iter));
    iter->index = 0;
    iter->count = 0;
    iter->map = map;
    iter->p_node = map->lists[0]->head;
    return iter;
}

void hashmap_iter_destroy(hashmap_iter_t *iter)
{
    assert(iter != NULL);
    assert((*iter) != NULL);

    free(*iter);
    iter = NULL;
}

int hashmap_iter_has_next(hashmap_iter_t iter)
{
    if (!iter)
        return -1;
    return iter->count < iter->map->len;
}

struct hashmap_pair *hashmap_iter_next(hashmap_iter_t iter)
{
    while (!iter->p_node) {
        iter->index++;
        if (iter->index >= iter->map->capacity)
            return NULL;
        iter->p_node = iter->map->lists[iter->index]->head;
    }
    struct hashmap_pair *p = BASIC2P(iter->p_node->data, struct hashmap_pair*);
    iter->p_node = iter->p_node->next;
    iter->count++;
    return p;
}
