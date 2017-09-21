/**
 * @author: luyuhuang
 * @brief: Data structure: a minimum heap
 */

 #ifndef _MIMHEAP_H_
 #define _MIMHEAP_H_

#include <stdio.h>
#include <stdbool.h>
#include "basic.h"

#define MINHEAP_INIT_CAPA       30

/*if arg1 < arg2, return 1(true) or return 0(false)*/
//typedef int (*minheap_lt)(void *, void*);
typedef bool (*minheap_lt)(basic_value_t, basic_value_t);
/*if arg1 == arg2, return 1(true) or return 0(false)*/
//typedef int (*minheap_eq)(void *, void*);

struct minheap {
    //void **heap_array;
    basic_value_t *heap_array;
    size_t capacity;
    size_t len;

    minheap_lt lt;
    //minheap_eq eq;
};

typedef struct minheap *minheap_t;

minheap_t minheap_create_for_all(minheap_lt lt, size_t init_capacity);
minheap_t minheap_create(minheap_lt lt);
void minheap_destroy(minheap_t *heap);

int minheap_add(minheap_t heap, basic_value_t data);
basic_value_t minheap_del(minheap_t heap, basic_value_t data);
basic_value_t minheap_top(minheap_t heap);
basic_value_t minheap_pop(minheap_t heap);
size_t minheap_len(minheap_t heap);


 #endif //_MIMHEAP_H_
