#include "../thread_pool.h"
#include <stdio.h>
#include <unistd.h>

void func(void *data)
{
    printf("thread %lx: %ld\n", pthread_self(), (long)data);
}

int main()
{
    THREAD_POOL_INST;
    for (long i = 0; i < 100; ++i) {
        thread_pool_push(THREAD_POOL_INST, func, (void*)i);
    }

    sleep(1);
    return 0;
}
