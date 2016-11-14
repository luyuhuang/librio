/**
 * @author: luyuhuang
 * @brief: Data structure: a chain table
 */

#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>

struct list_node {
    void *data;
    struct list_node *next;
};

/*if arg1 == arg2, return 1(true) or return 0(false)*/
typedef int (*list_eq)(void*, void*);

struct list {
    struct list_node *head;
    struct list_node *tail;
    size_t len;

    list_eq eq;
};

struct list_iter {
    struct list_node *p_node;
    size_t count;
};

typedef struct list *list_t;
typedef struct list_iter *list_iter_t;

list_t list_create(list_eq eq);
void list_destroy(list_t *list);

int list_insert_at_tail(list_t list, void *data);
int list_insert_at_head(list_t list, void *data);
int list_insert(list_t list, void *data, int index);

void *list_del_at_tail(list_t list);
void *list_del_at_head(list_t list);
void *list_del(list_t list, int index);

void *list_get_head(list_t list);
void *list_get_tail(list_t list);
void *list_get(list_t list, int index);

int list_find(list_t list, void *data);
int list_is_in(list_t list, void *data);

ssize_t list_len(list_t list);

list_iter_t list_iter_create(list_t list);
void list_iter_destroy(list_iter_t *it);

void *list_iter_next(list_iter_t it);
int list_iter_has_next(list_iter_t it);

#endif //_LIST_H_
