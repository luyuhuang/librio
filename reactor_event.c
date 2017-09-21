/**
 * @author: luyuhuang
 * @brief:
 */

#include "reactor_event.h"
#include "reactor.h"
#include "comm.h"
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

int64_t _m_int_hash(basic_value_t key)
{
    //return *(int*)key;
    return BASIC2L(key);
}

bool _m_int_equal(basic_value_t key1, basic_value_t key2)
{
    //return *(int*)key1 == *(int*)key2;
    return BASIC2L(key1) == BASIC2L(key2);
}

int64_t _m_uint64_hash(basic_value_t key)
{
    //return *(uint64_t*)key;
    return BASIC2U(key);
}

bool _m_uint64_equal(basic_value_t key1, basic_value_t key2)
{
    //return *(uint64_t*)key1 == *(uint64_t*)key2;
    return BASIC2U(key1) == BASIC2U(key2);
}

bool _h_timer_little(basic_value_t timer1, basic_value_t timer2)
{
    struct _h_timer *t1 = BASIC2P(timer1, struct _h_timer*);
    struct _h_timer *t2 = BASIC2P(timer2, struct _h_timer*);

    return t1->absolute_mtime < t2->absolute_mtime;
}

/*
int _h_timer_equal(void *timer1, void *timer2)
{
    struct _h_timer *t1 = (struct _h_timer*)timer1;
    struct _h_timer *t2 = (struct _h_timer*)timer2;
   
    return t1->eventid == t2->eventid;
}
*/

bool _l_revent_equal(basic_value_t event1, basic_value_t event2)
{
    struct revent *e1 = BASIC2P(event1, struct revent*);
    struct revent *e2 = BASIC2P(event2, struct revent*);

    return e1->eventid == e2->eventid;
}

int revent_on_timer(struct revent *event)
{
    assert(event->reason == REVENT_TIMEOUT);
    struct rtimer timer;
    timer.timer_id = event->timer_id;
    timer.mtime = event->mtime;
    timer.repeat = event->repeat;
    if (event->repeat) {
        reactor_add_timer(event->r, &timer, event->callback, event->data);
    }
    ((timer_cb)event->callback)(&timer, event->data);
    return 0;
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
                    ((accept_cb)event->callback)(&file, REACTER_ERR, &addr, len, event->data);
                    break;
                }
            }
            ((accept_cb)event->callback)(&file, fd, &addr, len, event->data);
        } while (1);
    }
    return 0;
}

int revent_on_connect(struct revent *event)
{
    struct rfile file;
    file.fd = event->fd;
    if (event->reason == REVENT_TIMEOUT) {
        ((connect_cb)event->callback)(&file, REVENT_TIMEOUT, event->data);
    } else if (event->reason == REVENT_READY){
        ((connect_cb)event->callback)(&file, event->fd, event->data);
    }
    return 0;
}

int revent_on_read(struct revent *event)
{
    struct rfile file;
    file.fd = event->fd;

    if (event->reason == REVENT_TIMEOUT) {
        ((read_cb)event->callback)(&file, NULL, REACTER_TIMEOUT, event->data);
    } else if (event->reason == REVENT_READY) {
        uint8_t *buffer = (uint8_t*)calloc(event->r->max_buffer_size, sizeof(uint8_t));
        ssize_t ret = thorough_read(event->fd, buffer, event->r->max_buffer_size);
        ((read_cb)event->callback)(&file, (void*)buffer, ret, event->data);
        free(buffer);
    }
    return 0;
}

int revent_on_write(struct revent *event)
{
    struct rfile file;
    file.fd = event->fd;

    if (event->reason == REVENT_TIMEOUT) {
        ((write_cb)event->callback)(&file, event->buffer, REACTER_TIMEOUT, event->data);
    } else if (event->reason == REVENT_READY) {
        ssize_t ret = thorough_write(event->fd, (uint8_t*)event->buffer, event->buffer_len);
        ((write_cb)event->callback)(&file, event->buffer, ret, event->data);
    }
    return 0;
}

int revent_on_signal(struct revent *event)
{
    assert(event->reason == REVENT_READY);
    struct rsignal signal;
    signal.sig = event->sig;
    ((signal_cb)event->callback)(&signal, event->data);
    return 0;
}
