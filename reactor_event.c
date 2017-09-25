/**
 * @author: luyuhuang
 * @brief:
 */

#include "reactor_event.h"
#include "reactor.h"
#include "comm.h"
#include "thread_pool.h"
#include "macro_tuple.h"
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

int64_t _m_int_hash(basic_value_t key)
{
    return BASIC2L(key);
}

bool _m_int_equal(basic_value_t key1, basic_value_t key2)
{
    return BASIC2L(key1) == BASIC2L(key2);
}

int64_t _m_uint64_hash(basic_value_t key)
{
    return BASIC2U(key);
}

bool _m_uint64_equal(basic_value_t key1, basic_value_t key2)
{
    return BASIC2U(key1) == BASIC2U(key2);
}

bool _h_timer_little(basic_value_t timer1, basic_value_t timer2)
{
    struct _h_timer *t1 = BASIC2P(timer1, struct _h_timer*);
    struct _h_timer *t2 = BASIC2P(timer2, struct _h_timer*);

    return t1->absolute_mtime < t2->absolute_mtime;
}

bool _l_revent_equal(basic_value_t event1, basic_value_t event2)
{
    struct revent *e1 = BASIC2P(event1, struct revent*);
    struct revent *e2 = BASIC2P(event2, struct revent*);

    return e1->eventid == e2->eventid;
}

static void _revent_on_timer_thread(void *arg) {
    struct revent *event;
    struct rtimer timer;

    GET_TUPLE_2(arg, event, timer);

    ((timer_cb)event->callback)(&timer, event->data);

    DELETE_TUPLE(arg);
    if (event->delete_while_done)
        free(event);
}

int revent_on_timer(struct revent *event)
{
    assert(event->reason == REVENT_TIMEOUT);
    /*
    struct rtimer *timer = (struct rtimer*)malloc(sizeof(struct rtimer));
    timer->timer_id = event->timer_id;
    timer->mtime = event->mtime;
    timer->repeat = event->repeat;
    */
    struct rtimer timer;
    timer.timer_id = event->timer_id;
    timer.mtime = event->mtime;
    timer.repeat = event->repeat;
    
    if (event->repeat) {
        reactor_add_timer(event->r, &timer, event->callback, event->data);
    }

    void *tuple = NEW_TUPLE_2(event, timer);

#if 0
    thread_pool_push(THREAD_POOL_INST, LAMBDA(void, (void *arg) {
        struct revent *event;
        struct rtimer timer;

        GET_TUPLE_2(arg, event, timer);

        ((timer_cb)event->callback)(&timer, event->data);

        DELETE_TUPLE(arg);
        if (event->delete_while_done)
            free(event);
    }), tuple);
#else
    thread_pool_push(THREAD_POOL_INST, _revent_on_timer_thread, (void*)tuple);
#endif

    //((timer_cb)event->callback)(&timer, event->data);
    return 0;
}

static void _revent_on_accept_thread(void *arg) {
    struct revent *event;
    struct rfile file;
    int fd;
    struct sockaddr addr;
    socklen_t len;
    
    GET_TUPLE_5(arg, event, file, fd, addr, len);
    ((accept_cb)event->callback)(&file, fd, &addr, len, event->data);

    DELETE_TUPLE(arg);
    if (event->delete_while_done)
        free(event);
}


int revent_on_accept(struct revent *event)
{
    struct sockaddr addr;
    socklen_t len;
    int fd;
    struct rfile file;
    file.fd = event->fd;

    if (event->reason == REVENT_TIMEOUT) {
        ((accept_cb)event->callback)(&file, REACTER_TIMEOUT, NULL, 0, event->data);
    } else if (event->reason == REVENT_READY){
        do {
            fd = accept(event->fd, &addr, &len);
            if (fd < 0) {
                if (errno == EAGAIN)
                    break;
                else if (errno == EINTR)
                    continue;
                else {
                    //((accept_cb)event->callback)(&file, REACTER_ERR, &addr, len, event->data);
                    void *tuple = NEW_TUPLE_6(event, file, (int)REACTER_ERR, addr, len, event->data);
                    thread_pool_push(THREAD_POOL_INST, _revent_on_accept_thread, tuple);
                    break;
                }
            }
            void *tuple = NEW_TUPLE_5(event, file, fd, addr, len);
            thread_pool_push(THREAD_POOL_INST, _revent_on_accept_thread, tuple);
            //((accept_cb)event->callback)(&file, fd, &addr, len, event->data);
        } while (1);
    }
    return 0;
}

static void _revent_on_connect_thread(void *arg) {
    struct revent *event;
    struct rfile file;
    int fd;

    GET_TUPLE_3(arg, event, file, fd);
    ((connect_cb)event->callback)(&file, REVENT_TIMEOUT, event->data);

    DELETE_TUPLE(arg);
    if (event->delete_while_done)
        free(event);
}


int revent_on_connect(struct revent *event)
{

    struct rfile file;
    file.fd = event->fd;
    if (event->reason == REVENT_TIMEOUT) {
        void *tuple = NEW_TUPLE_3(event, file, (int)REVENT_TIMEOUT);
        thread_pool_push(THREAD_POOL_INST, _revent_on_connect_thread, tuple);
        //((connect_cb)event->callback)(&file, REVENT_TIMEOUT, event->data);
    } else if (event->reason == REVENT_READY){
        void *tuple = NEW_TUPLE_3(event, file, event->fd);
        thread_pool_push(THREAD_POOL_INST, _revent_on_connect_thread, tuple);
        //((connect_cb)event->callback)(&file, event->fd, event->data);
    }
    return 0;
}

static void _revent_on_read_thread(void *arg) {
    struct revent *event;
    struct rfile file;
    void *buffer;
    int ret;

    GET_TUPLE_4(arg, event, file, buffer, ret);
    if (((read_cb)event->callback)(&file, buffer, ret, event->data) == 0);
        free(buffer);

    DELETE_TUPLE(arg);
    if (event->delete_while_done)
        free(event);
}

int revent_on_read(struct revent *event)
{

    struct rfile file;
    file.fd = event->fd;

    if (event->reason == REVENT_TIMEOUT) {
        void *tuple = NEW_TUPLE_4(event, file, (void*)NULL, (int)REACTER_TIMEOUT);
        thread_pool_push(THREAD_POOL_INST, _revent_on_read_thread, tuple);
        //((read_cb)event->callback)(&file, NULL, REACTER_TIMEOUT, event->data);
    } else if (event->reason == REVENT_READY) {
        uint8_t *buffer = (uint8_t*)calloc(event->r->max_buffer_size, sizeof(uint8_t));
        ssize_t ret = thorough_read(event->fd, buffer, event->r->max_buffer_size);

        void *tuple = NEW_TUPLE_4(event, file, (void*)buffer, ret);
        thread_pool_push(THREAD_POOL_INST, _revent_on_read_thread, tuple);
        /*
        if (((read_cb)event->callback)(&file, (void*)buffer, ret, event->data) == 0);
            free(buffer);
        */
    }
    return 0;
}

static void _revent_on_write_thread(void *arg) {
    struct revent *event;
    struct rfile file;
    int ret;

    GET_TUPLE_3(arg, event, file, ret);
    ((write_cb)event->callback)(&file, event->buffer, ret, event->data);

    DELETE_TUPLE(arg);
    if (event->delete_while_done)
        free(event);
}

int revent_on_write(struct revent *event)
{

    struct rfile file;
    file.fd = event->fd;

    if (event->reason == REVENT_TIMEOUT) {
        void *tuple = NEW_TUPLE_3(event, file, (int)REACTER_TIMEOUT);
        thread_pool_push(THREAD_POOL_INST, _revent_on_write_thread, tuple);
        //((write_cb)event->callback)(&file, event->buffer, REACTER_TIMEOUT, event->data);
    } else if (event->reason == REVENT_READY) {
        ssize_t ret = thorough_write(event->fd, (uint8_t*)event->buffer, event->buffer_len);

        void *tuple = NEW_TUPLE_3(event, file, ret);
        thread_pool_push(THREAD_POOL_INST, _revent_on_write_thread, tuple);
        //((write_cb)event->callback)(&file, event->buffer, ret, event->data);
    }
    return 0;
}


static void _revent_on_signal_thread(void *arg) {
    struct revent *event;
    struct rsignal signal;

    GET_TUPLE_2(arg, event, signal);
    ((signal_cb)event->callback)(&signal, event->data);

    DELETE_TUPLE(arg);
    if (event->delete_while_done)
        free(event);
}

int revent_on_signal(struct revent *event)
{

    assert(event->reason == REVENT_READY);
    struct rsignal signal;
    signal.sig = event->sig;

    void *tuple = NEW_TUPLE_2(event, signal);
    thread_pool_push(THREAD_POOL_INST, _revent_on_signal_thread, tuple);
    //((signal_cb)event->callback)(&signal, event->data);
    return 0;
}
