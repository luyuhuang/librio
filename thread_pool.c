/**
 * @author: luyuhuang
 * @brief: a thread pool
 */

#include "thread_pool.h"
#include <string.h>

#define QUEUE_EMPTY(pool) ((pool)->tq_head == (pool)->tq_tail)
#define QUEUE_FULL(pool) (((pool)->tq_tail + 1) % (pool)->tq_len == (pool)->tq_head)

#ifndef TASK_QUEUE_DONOT_RESIZE
static int _queue_resize(struct thread_pool *pool)
{
    size_t new_len = pool->tq_len * 2;
    struct task *new_queue = (struct task*)malloc(sizeof(struct task) * new_len);
    if (pool->tq_head > pool->tq_tail) {
        int tlen = pool->tq_tail + 1;
        memcpy(new_queue, pool->task_queue, sizeof(struct task) * tlen);
        int hlen = pool->tq_len = pool->tq_head;
        int pos = new_len - hlen;
        memcpy(new_queue + pos, pool->task_queue + pool->tq_head, sizeof(struct task) * hlen);
        pool->tq_head = pos;
    } else {
        memcpy(new_queue, pool->task_queue, sizeof(struct task) * pool->tq_len);
    }
    free(pool->task_queue);
    pool->tq_len = new_len;
    pool->task_queue = new_queue;

    return 0;
}
#endif //TASK_QUEUE_DONOT_RESIZE

static int _queue_push(struct thread_pool *pool, task_func task, void *data)
{
    LOCK(&pool->queue_lock);

    if (QUEUE_FULL(pool)) {
#ifdef TASK_QUEUE_DONOT_RESIZE
        UNLOCK(&pool->queue_lock);
        return -1;
#else
        _queue_resize(pool);
#endif
    }
    pool->task_queue[pool->tq_tail].func = task;
    pool->task_queue[pool->tq_tail].data = data;
    pool->tq_tail = (pool->tq_tail + 1) % pool->tq_len;

    UNLOCK(&pool->queue_lock);
    return 0;
}

//static task_func _queue_pop(struct thread_pool *pool)
static int _queue_pop(struct thread_pool *pool, struct task *ret)
{
    LOCK(&pool->queue_lock);

    if (QUEUE_EMPTY(pool)) {
        UNLOCK(&pool->queue_lock);
        return -1;
    }

    ret->func = pool->task_queue[pool->tq_head].func;
    ret->data = pool->task_queue[pool->tq_head].data;
    pool->tq_head = (pool->tq_head + 1) % pool->tq_len;

    UNLOCK(&pool->queue_lock);
    return 0;
}


static struct thread_pool *_g_thread_pool_instance = NULL;
static lock_t _g_instance_lock = LOCK_INITIALIZER;
static void *_thread_dealer(void *arg);

static struct thread_pool *
_thread_pool_create()
{
    struct thread_pool *pool = (struct thread_pool*)malloc(sizeof(struct thread_pool));

    pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * THREAD_COUNT);
    pool->thread_num = THREAD_COUNT;

    pool->task_queue = (struct task*)malloc(sizeof(struct task) * TASK_QUEUE_INIT_LEN);
    pool->tq_len = TASK_QUEUE_INIT_LEN;
    pool->tq_head = pool->tq_tail = 0;

    LOCK_INIT(&pool->queue_lock);
    sem_init(&pool->queue_sem, 0, 0);

    for (int i = 0; i < pool->thread_num; ++i) {
        pthread_create(pool->threads + i, NULL, _thread_dealer, (void*)pool);
        pthread_detach(pool->threads[i]);
    }

    return pool;
}

struct thread_pool *thread_pool_instance()
{
    if (_g_thread_pool_instance == NULL) {
        LOCK(&_g_instance_lock);
        _g_thread_pool_instance = _thread_pool_create();
        UNLOCK(&_g_instance_lock);
    }

    return _g_thread_pool_instance;
}

int thread_pool_push(struct thread_pool *pool, task_func task, void *data)
{
    if (_queue_push(pool, task, data) != 0)
        return -1;

    sem_post(&pool->queue_sem);
    return 0;
}

static int _thread_pool_pop(struct thread_pool *pool, struct task *ret)
{
    sem_wait(&pool->queue_sem);

    if (_queue_pop(pool, ret) != 0)
        return -1;
    return 0;
}

static void *_thread_dealer(void *arg)
{
    struct thread_pool *pool = (struct thread_pool*)arg;
    struct task t;
    while (1) {
        _thread_pool_pop(pool, &t);
        t.func(t.data);
    }
    return NULL;
}

