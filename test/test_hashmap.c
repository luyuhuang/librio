#include "../hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST_ARRAY_SIZE 100000

int *g_array = NULL;
hashmap_t g_map = NULL;

static int _hs(void *data)
{
    return *(int*)data;
}

static int _eq(void *data1, void *data2)
{
    return *(int*)data1 == *(int*)data2;
}


static int _test_insert_into_hashmap()
{
    int index = 0;
    srand((unsigned int)clock());

    clock_t t1 = clock();
    for (int i = 0; i < TEST_ARRAY_SIZE; i++) {
        index = rand() % TEST_ARRAY_SIZE;
        hashmap_add(g_map, g_array + index, g_array + index);
    }
    clock_t t2 = clock();

    printf("insert %d consume %lf(s)\n", TEST_ARRAY_SIZE, (t2 - t1) / (CLOCKS_PER_SEC + 0.0));

    return 0;
}

static int _test_find_from_hashmap()
{
    int index = 0;
    srand((unsigned int)clock());

    clock_t t1 = clock();
    for (int i = 0; i < TEST_ARRAY_SIZE; i++) {
        index = rand() % TEST_ARRAY_SIZE;
        hashmap_get_value(g_map, g_array + index);
    }
    clock_t t2 = clock();

    printf("find %d consume %lf(s)\n", TEST_ARRAY_SIZE, (t2 - t1) / (CLOCKS_PER_SEC + 0.0));

    return 0;
}

static void _init_env()
{
    g_array = calloc(TEST_ARRAY_SIZE, sizeof(int));
    for (int i = 0; i < TEST_ARRAY_SIZE; i++)
        g_array[i] = i;
    g_map = hashmap_create(_hs, _eq);
}

static void _free_env()
{
    free(g_array);
    hashmap_destroy(&g_map);
}

int main()
{
    _init_env();

    _test_insert_into_hashmap();
    printf("test insert into hashmap: OK\n");
    _test_find_from_hashmap();
    printf("test find from hashmap: OK\n");

    _free_env();
    return 0;
}
