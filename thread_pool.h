/**
 * @author: luyuhuang
 * @brief: a thread pool
 */

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include "comm.h"

#define THREAD_COUNT 1
#define TASK_QUEUE_INIT_LEN 32


typedef void (*task_func)(void*);

struct task {
    task_func func;
    void *data;
};

struct thread_pool {
    pthread_t *threads;
    size_t thread_num;

    //task_func *task_queue;
    struct task *task_queue;
    size_t tq_len;
    int tq_head;
    int tq_tail;

    lock_t queue_lock;
    sem_t queue_sem;    //sem value always equal the number of task in queue.
};

struct thread_pool *thread_pool_instance();

#define THREAD_POOL_INST (thread_pool_instance())

int thread_pool_push(struct thread_pool *pool, task_func task, void *data);

#endif //_THREAD_POOL_H_
