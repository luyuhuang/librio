/**
 * @author: luyuhuang
 * @brief: Data structure: a minimum heap
 */
#include "minheap.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

minheap_t minheap_create_for_all(minheap_lt lt, size_t init_capacity)
{
    if (!lt)
        return NULL;
    if (init_capacity <= 0)
        return NULL;

    minheap_t heap = (struct minheap*)calloc(1, sizeof(struct minheap));
    assert(heap != NULL);

    heap->heap_array = (basic_value_t*)calloc(init_capacity, sizeof(basic_value_t));
    assert(heap->heap_array != NULL);

    heap->capacity = init_capacity;
    heap->len = 0;
    
    heap->lt = lt;

    return heap;
}

minheap_t minheap_create(minheap_lt lt)
{
    return minheap_create_for_all(lt, MINHEAP_INIT_CAPA);
}

void minheap_destroy(minheap_t *heap)
{
    if (!heap || !*heap)
        return;
    free((*heap)->heap_array);
    free(*heap);
    *heap = NULL;
}

static int _minheap_adjust_down(minheap_t heap, int hole)
{
    if (!heap || hole < 0)
        return -1;
    if (heap->len == 0)
        return 0;

    basic_value_t tmp = heap->heap_array[hole];
    int child = 0;
    for (;(hole*2 + 1) <= heap->len - 1; hole = child) {
        child = hole * 2 + 1;
        if (child < heap->len - 1 && heap->lt(heap->heap_array[child + 1], heap->heap_array[child]))
            child++;
        if (heap->lt(heap->heap_array[child], tmp))
            heap->heap_array[hole] = heap->heap_array[child];
        else break;
    }
    heap->heap_array[hole] = tmp;
    return 0;
}

static int _minheap_resize(minheap_t heap)
{
    if (!heap)
        return -1;

    heap->capacity *= 2;
    basic_value_t *new_array = (basic_value_t*)calloc(heap->capacity, sizeof(basic_value_t));
    assert(new_array != NULL);

    bzero(new_array, sizeof(void*) * heap->capacity);
    memcpy(new_array, heap->heap_array, sizeof(void*) * heap->capacity);
    free(heap->heap_array);
    heap->heap_array = new_array;
    return 0;
}

int minheap_add(minheap_t heap, basic_value_t data)
{
    if (!heap)
        return -1;

    if (heap->len >= heap->capacity)
        _minheap_resize(heap);

    int hole = heap->len++;
    int parent = 0;
    for (;hole > 0; hole = parent) {
        parent = (hole - 1) / 2;
        if (heap->lt(heap->heap_array[parent], heap->heap_array[hole]))
            break;
        heap->heap_array[hole] = heap->heap_array[parent];
    }
    heap->heap_array[hole] = data;
    return 0;
}

static bool _minheap_eq(minheap_t heap, basic_value_t v1, basic_value_t v2)
{
    assert(heap);
    return !heap->lt(v1, v2) && !heap->lt(v2, v1);
}

basic_value_t minheap_del(minheap_t heap, basic_value_t data)
{
    if (!heap)
        return BASIC_NULL;

    int del_index = -1;
    for (int i = 0; i < heap->len; i++) {
        //if (heap->eq(heap->heap_array[i], data))
        if (_minheap_eq(heap, heap->heap_array[i], data))
            del_index = i;
    }
    if (del_index == -1)
        return BASIC_NULL;

    basic_value_t d = heap->heap_array[del_index];
    heap->heap_array[del_index] = heap->heap_array[--heap->len];
    return _minheap_adjust_down(heap, del_index) == 0 ? d : BASIC_NULL;
}

basic_value_t minheap_top(minheap_t heap)
{
    if (!heap || heap->len == 0)
        return BASIC_NULL;
    return heap->heap_array[0];
}

basic_value_t minheap_pop(minheap_t heap)
{
    if (!heap || heap->len == 0)
        return BASIC_NULL;
    basic_value_t top = heap->heap_array[0];
    heap->heap_array[0] = heap->heap_array[--heap->len];
    return _minheap_adjust_down(heap, 0) == 0 ? top : BASIC_NULL;
}

size_t minheap_len(minheap_t heap)
{
    return heap->len;
}
