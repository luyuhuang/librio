#include "../hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST_TIMES 100000

hashmap_t g_map = NULL;

static int64_t _hs(basic_value_t data)
{
    return BASIC2L(data);
}

static bool _eq(basic_value_t data1, basic_value_t data2)
{
    return BASIC2L(data1) == BASIC2L(data2);
}


static int _test_insert_into_hashmap()
{
    srand((unsigned int)clock());

    clock_t t1 = clock();
    for (int i = 0; i < TEST_TIMES; ++i) {
        int key = rand();
        int value = rand();
        hashmap_add(g_map, L2BASIC(key), L2BASIC(value));
    }
    clock_t t2 = clock();

    printf("insert %d consume %lf(s)\n", TEST_TIMES, (t2 - t1) / (CLOCKS_PER_SEC + 0.0));

    return 0;
}

static int _test_find_from_hashmap()
{
    srand((unsigned int)clock());

    clock_t t1 = clock();
    for (int i = 0; i < TEST_TIMES; ++i) {
        int key = rand();
        hashmap_get_value(g_map, L2BASIC(key));
    }
    clock_t t2 = clock();

    printf("find %d consume %lf(s)\n", TEST_TIMES, (t2 - t1) / (CLOCKS_PER_SEC + 0.0));

    return 0;
}

static int _test_traversals_hashmap()
{
    hashmap_iter_t iter = hashmap_iter_create(g_map);
    struct hashmap_pair *p;
    int i = 0;
    while ((p = hashmap_iter_next(iter)) != NULL) {
        if (i < 30)
            printf("%ld: %ld\n", BASIC2L(p->key), BASIC2L(p->value));
        ++i;
    }
    hashmap_iter_destroy(&iter);
    return 0;
}

static void _init_env()
{
    g_map = hashmap_create_for_all(_hs, _eq, 100000, 0.5f);
}

static void _free_env()
{
    hashmap_destroy(&g_map);
}

int main()
{
    _init_env();

    _test_insert_into_hashmap();
    //printf("test insert into hashmap: OK\n");
    _test_find_from_hashmap();
    //printf("test find from hashmap: OK\n");
    _test_traversals_hashmap();

    _free_env();
    return 0;
}
