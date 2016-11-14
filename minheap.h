/**
 * @author: luyuhuang
 * @brief: Data structure: a minimum heap
 */

 #ifndef _MIMHEAP_H_
 #define _MIMHEAP_H_

#include <stdio.h>

#define MINHEAP_INIT_CAPA       30

/*if arg1 < arg2, return 1(true) or return 0(false)*/
typedef int (*minheap_lt)(void *, void*);
/*if arg1 == arg2, return 1(true) or return 0(false)*/
typedef int (*minheap_eq)(void *, void*);

struct minheap {
    void **heap_array;
    size_t capacity;
    size_t len;

    minheap_lt lt;
    minheap_eq eq;
};

typedef struct minheap *minheap_t;

minheap_t minheap_create_for_all(minheap_lt lt, minheap_eq eq, size_t init_capacity);
minheap_t minheap_create(minheap_lt lt, minheap_eq eq);
void minheap_destroy(minheap_t *heap);

int minheap_add(minheap_t heap, void *data);
void *minheap_del(minheap_t heap, void *data);
void *minheap_top(minheap_t heap);
void *minheap_pop(minheap_t heap);
size_t minheap_len(minheap_t heap);


 #endif //_MIMHEAP_H_
