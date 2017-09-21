/**
 * @author: luyuhuang
 * @brief: chain table using macro implementation
 */

#ifndef _MACRO_LIST_H_
#define _MACRO_LIST_H_

#define LIST(type)      \
    struct {            \
        type *head;     \
        type *tail;     \
    }

#define LIST_INITIALIZER {NULL, NULL}
#define LIST_INIT(list)         \
    do {                        \
        (list)->head = NULL;    \
        (list)->tail = NULL;    \
    } while (0)

#define LIST_BEGIN(list) ((list)->head)
#define LIST_END(list) NULL
#define LIST_EMPTY(list) ((list)->head == NULL)
#define LIST_NEXT(node) ((node)->__next__)
#define LIST_PREV(node) ((node)->__prev__)

#define LIST_FOREACH(var, list)     \
    for ((var) = LIST_BEGIN(list); (var) != LIST_END(list); (var) = LIST_NEXT(var))

#define LIST_INSERT_AFTER(list, listnode, node)                     \
    do {                                                            \
        if (((node)->__next__ = (listnode)->__next__) != NULL) {    \
            (listnode)->__next__->__prev__ = (node);                \
            (listnode)->__next__ = (node);                          \
            (node)->__prev__ = (listnode);                          \
        } else {                                                    \
            (listnode)->__next__ = (node);                          \
            (node)->__prev__ = (listnode);                          \
            (list)->tail = (node);                                  \
        }                                                           \
    } while (0)

#define LIST_INSERT_BEFORE(list, listnode, node)                    \
    do {                                                            \
        if (((node)->__prev__ = (listnode)->__prev__) != NULL) {    \
            (listnode)->__prev__->__next__ = (node);                \
            (listnode)->__prev__ = (node);                          \
            (node)->__next__ = (listnode);                          \
        } else {                                                    \
            (listnode)->__prev__ = (node);                          \
            (node)->__next__ = (listnode);                          \
            (list)->head = (node);                                  \
        }                                                           \
    } while (0)

#define LIST_INSERT_AT_HEAD(list, node)                     \
    do {                                                    \
        if (LIST_EMPTY(list)) {                             \
            (list)->head = (list)->tail = (node);           \
            (node)->__prev__ = NULL;                        \
            (node)->__next__ = NULL;                        \
        } else {                                            \
            LIST_INSERT_BEFORE(list, (list)->head, node);   \
        }                                                   \
    } while (0)

#define LIST_INSERT_AT_TAIL(list, node)                     \
    do {                                                    \
        if (LIST_EMPTY(list)) {                             \
            (list)->head = (list)->tail = (node);           \
            (node)->__prev__ = NULL;                        \
            (node)->__next__ = NULL;                        \
        } else {                                            \
            LIST_INSERT_AFTER(list, (list)->tail, node);    \
        }                                                   \
    } while (0)

#define LIST_ERASE(list, node)                              \
    do {                                                    \
        if ((list)->head == (node)) {                       \
            (list)->head = (list)->head->__next__;          \
            if ((list)->head)                               \
                (list)->head->__prev__ = NULL;              \
         } else if ((list)->tail == (node)) {               \
            (list)->tail = (list)->tail->__prev__;          \
            if ((list)->tail)                               \
                (list)->tail->__next__ = NULL;              \
        } else {                                            \
            (node)->__prev__->__next__ = (node)->__next__;  \
            (node)->__next__->__prev__ = (node)->__prev__;  \
        }                                                   \
    } while (0)



#define SLIST(type) \
    struct {        \
        type *head; \
        type *tail; \
    }

#define SLIST_INITIALIZER {NULL, NULL}
#define SLIST_INIT(slist)       \
    do {                        \
        (slist)->head = NULL;   \
        (slist)->tail = NULL;   \
    } while (0)

#define SLIST_BEGIN(slist) ((slist)->head)
#define SLIST_END(slist) NULL
#define SLIST_EMPTY(slist) ((slist)->head == NULL)
#define SLIST_NEXT(node) ((node)->__next__)

#define SLIST_FOREACH(var, slist)     \
    for ((var) = SLIST_BEGIN(slist); (var) != SLIST_END(slist); (var) = SLIST_NEXT(var))

#define SLIST_INSERT_AFTER(slist, slistnode, node)  \
    do {                                            \
        (node)->__next__ = (slistnode)->__next__;   \
        (slistnode)->__next__ = (node);             \
        if ((slistnode) == (slist)->tail)           \
            (slist)->tail = (node);                 \
    } while (0)

#define SLIST_INSERT_AT_HEAD(slist, node)   \
    do {                                    \
        (node)->__next__ = (slist)->head;   \
        (slist)->head = (node);             \
    } while (0)

#define SLIST_INSERT_AT_TAIL(slist, node)                   \
    do {                                                    \
        if (SLIST_EMPTY(slist)) {                           \
            (slist)->head = (slist)->tail = (node);         \
            (node)->__next__ = NULL;                        \
        } else {                                            \
            SLIST_INSERT_AFTER(slist, (slist)->tail, node); \
        }                                                   \
    } while (0)

#define SLIST_ERASE_HEAD(slist)                     \
    do {                                            \
        (slist)->head = (slist)->head->__next__;    \
    } while (0)

#define SLIST_ERASE_AFTER(slist, node)                  \
    do {                                                \
        (node)->__next__ = (node)->__next__->__next__;  \
        if ((node)->__next__ == NULL)                   \
            (slist)->tail = (node);                     \
    } while (0)

#endif //_MACRO_LIST_H_
