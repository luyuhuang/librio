/**
 * @author: luyuhuang
 * @brief: Data structure: a minimum heap
 */
#include "minheap.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

minheap_t minheap_create_for_all(minheap_lt lt, minheap_eq eq, size_t init_capacity)
{
    if (!lt || !eq)
        return NULL;
    if (init_capacity <= 0)
        return NULL;

    minheap_t heap = (struct minheap*)calloc(1, sizeof(struct minheap));
    assert(heap != NULL);

    heap->heap_array = (void**)calloc(init_capacity, sizeof(void*));
    assert(heap->heap_array != NULL);

    heap->capacity = init_capacity;
    heap->len = 0;
    
    heap->lt = lt;
    heap->eq = eq;

    return heap;
}

minheap_t minheap_create(minheap_lt lt, minheap_eq eq)
{
    return minheap_create_for_all(lt, eq, MINHEAP_INIT_CAPA);
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

    void *tmp = heap->heap_array[hole];
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
    void **new_array = (void**)calloc(heap->capacity, sizeof(void*));
    assert(new_array != NULL);

    bzero(new_array, sizeof(void*) * heap->capacity);
    memcpy(new_array, heap->heap_array, sizeof(void*) * heap->capacity);
    free(heap->heap_array);
    heap->heap_array = new_array;
    return 0;
}

int minheap_add(minheap_t heap, void *data)
{
    if (!heap || !data)
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

void *minheap_del(minheap_t heap, void *data)
{
    if (!heap || !data)
        return NULL;

    int del_index = -1;
    for (int i = 0; i < heap->len; i++) {
        if (heap->eq(heap->heap_array[i], data))
            del_index = i;
    }
    if (del_index == -1)
        return NULL;

    void *d = heap->heap_array[del_index];
    heap->heap_array[del_index] = heap->heap_array[--heap->len];
    return _minheap_adjust_down(heap, del_index) == 0 ? d : NULL;
}

void *minheap_top(minheap_t heap)
{
    if (!heap || heap->len == 0)
        return NULL;
    return heap->heap_array[0];
}

void *minheap_pop(minheap_t heap)
{
    if (!heap || heap->len == 0)
        return NULL;
    void *top = heap->heap_array[0];
    heap->heap_array[0] = heap->heap_array[--heap->len];
    return _minheap_adjust_down(heap, 0) == 0 ? top : NULL;
}

size_t minheap_len(minheap_t heap)
{
    return heap->len;
}
