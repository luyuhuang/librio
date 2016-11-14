/**
 * @author: luyuhuang
 * @brief: Data structure: a chain table
 */

#include "list.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

list_t list_create(list_eq eq)
{
    if (!eq)
        return NULL;

    list_t list = (struct list*)calloc(1, sizeof(struct list));
    assert(list != NULL);

    list->head = list->tail = NULL;
    list->len = 0;
    list->eq = eq;
    return list;
}

void list_destroy(list_t *list)
{
    assert(list != NULL);
    assert((*list) != NULL);
    struct list_node *p = (*list)->head;
    while (p != NULL) {
        struct list_node *node = p;
        p = p->next;
        free(node);
    }
    free(*list);
    *list = NULL;
}

int list_insert_at_head(list_t list, void *data)
{
    if (!list)
        return 0;

    struct list_node *node = (struct list_node*)calloc(1, sizeof(struct list_node));
    assert(node != NULL);
    node->data = data;
    node->next = NULL;

    if (list->len == 0) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head = node;
    }
    list->len++;
    return 0;
}

int list_insert_at_tail(list_t list, void *data)
{
    if (!list)
        return 0;

    struct list_node *node = (struct list_node*)calloc(1, sizeof(struct list_node));
    assert(node != NULL);
    node->data = data;
    node->next = NULL;

    if (list->len == 0) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->len++;
    return 0;
}

int list_insert(list_t list, void *data, int index)
{
    if (!list || index < 0)
        return -1;

    if (index == 0)
        return list_insert_at_head(list, data);
    else if (index == list->len)
        return list_insert_at_tail(list, data);
    else {
        struct list_node *node = (struct list_node*)calloc(1, sizeof(struct list_node));
        assert(node != NULL);
        node->data = data;
        node->next = NULL;

        struct list_node *p = list->head;
        for (int i = 0; i < index - 1; i++)
            p = p->next;
        p->next = node;
        node->next = p->next;
        list->len++;
        return 0;
    }
}

void *list_del_at_head(list_t list)
{
    if (!list)
        return NULL;
    struct list_node *p = list->head;
    list->head = list->head->next;
    void *data = p->data;
    free(p);
    list->len--;
    if (list->len == 0)
        list->tail = NULL;
    return data;
}

void *list_del_at_tail(list_t list)
{
    if (!list)
        return NULL;
    if (list->len == 1)
        return list_del_at_head(list);

    struct list_node *p = list->head;
    while (p->next != list->tail)
        p = p->next;
    void *data = list->tail->data;
    free(list->tail);
    p->next = NULL;
    list->tail = p;
    list->len--;
    return data;
}

void *list_del(list_t list, int index)
{
    if (!list || index < 0)
        return NULL;

    if (index == 0)
        return list_del_at_head(list);
    if (index == list->len - 1)
        return list_del_at_tail(list);

    struct list_node *p = list->head;
    for (int i = 0; i < index - 1; i++) {
        p = p->next;
    }
    struct list_node *node = p->next;
    void *data = node->data;
    p->next = p->next->next;
    free(node);
    list->len--;
    return data;
}

void *list_get_head(list_t list)
{
    if (!list)
        return NULL;
    return list->head->data;
}

void *list_get_tail(list_t list)
{
    if (!list)
        return NULL;
    return list->tail->data;
}

void *list_get(list_t list, int index)
{
    if (!list || index < 0)
        return NULL;
    if (index == 0)
        return list_get_head(list);
    if (index == list->len-1)
        return list_get_tail(list);

    struct list_node *p = list->head;
    for (int i = 0; i < index; i++)
        p = p->next;
    return p->data;
}

int list_find(list_t list, void *data)
{
    struct list_node *p = list->head;
    int i = 0;
    while (p != NULL) {
        if (list->eq(p->data, data))
            return i;
        i++;
        p = p->next;
    }
    return -1;
}

int list_is_in(list_t list, void *data)
{
    return list_find(list, data) == -1 ? 0 : 1;
}

ssize_t list_len(list_t list)
{
    if (!list)
        return -1;
    return list->len;
}


list_iter_t list_iter_create(list_t list)
{
    list_iter_t it = (struct list_iter*)calloc(1, sizeof(struct list_iter));
    assert(it != NULL);
    it->p_node = list->head;
    it->count = 0;
    return it;
}

void list_iter_destroy(list_iter_t *it)
{
    assert(it != NULL);
    assert((*it) != NULL);
    free(*it);
    *it = NULL;
}

void *list_iter_next(list_iter_t it)
{
    if (it->p_node == NULL)
        return NULL;
    void *data = it->p_node->data;
    it->p_node = it->p_node->next;
    it->count++;
    return data;
}

int list_iter_has_next(list_iter_t it)
{
    return it->p_node != NULL;
}
