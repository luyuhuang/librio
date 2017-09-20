#include "../macro_list.h"
#include <stdio.h>
#include <stdlib.h>


struct node {
    int a;
    struct node *__next__;
    struct node *__prev__;
};

LIST(struct node) g_list = LIST_INITIALIZER;
SLIST(struct node) g_slist = SLIST_INITIALIZER;

static int _test_list_insert()
{
    struct node *n, *n1;
    for (int i = 0; i < 16; ++i) {
        n = (struct node*)malloc(sizeof(struct node));
        n->a = i;
        LIST_INSERT_AT_TAIL(&g_list, n);
    }

    for (int i = 10; i > 0; --i) {
        n = (struct node*)malloc(sizeof(struct node));
        n->a = i;
        LIST_INSERT_AT_HEAD(&g_list, n);
    }

    LIST_FOREACH(n, &g_list) {
        if (n->a == 12) break;
    }

    n1 = (struct node*)malloc(sizeof(struct node));
    n1->a = 123;
    LIST_INSERT_AFTER(&g_list, n, n1);

    n = (struct node*)malloc(sizeof(struct node));
    n->a = 345;
    LIST_INSERT_BEFORE(&g_list, n1, n);

    LIST_FOREACH(n, &g_list) {
        printf("%d\n", n->a);
    }

    return 0;
}


static int _test_list_erase()
{
    struct node *n, *n1;

    LIST_ERASE(&g_list, g_list.head);
    LIST_ERASE(&g_list, g_list.tail);

    LIST_FOREACH(n, &g_list) {
        if (n->a == 11 || n->a == 13) {
            LIST_ERASE(&g_list, n);
        }
    }

    LIST_FOREACH(n, &g_list) {
        printf("%d\n", n->a);
    }

    return 0;

}

static int _test_slist_insert()
{
    struct node *n, *n1;
    for (int i = 0; i < 16; ++i) {
        n = (struct node*)malloc(sizeof(struct node));
        n->a = i;
        SLIST_INSERT_AT_TAIL(&g_slist, n);
    }

    for (int i = 10; i > 0; --i) {
        n = (struct node*)malloc(sizeof(struct node));
        n->a = i;
        SLIST_INSERT_AT_HEAD(&g_slist, n);
    }

    LIST_FOREACH(n, &g_slist) {
        if (n->a == 12) break;
    }

    n1 = (struct node*)malloc(sizeof(struct node));
    n1->a = 123;
    SLIST_INSERT_AFTER(&g_slist, n, n1);

    SLIST_FOREACH(n, &g_slist) {
        printf("%d\n", n->a);
    }

    return 0;
}

static int _test_slist_erase()
{
    struct node *n;
    SLIST_ERASE_HEAD(&g_slist);
    SLIST_ERASE_HEAD(&g_slist);
    SLIST_ERASE_HEAD(&g_slist);

    SLIST_FOREACH(n, &g_slist) {
        printf("%d\n", n->a);
    }
    return 0;
}


int main()
{
    _test_list_insert();
    printf("test list insert succeed\n");
    _test_list_erase();
    printf("test list erase succeed\n");
    _test_slist_insert();
    printf("test slist insert succeed\n");
    _test_slist_erase();
    printf("test slist erase succeed\n");

    return 0;
}
