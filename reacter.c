/**
 * @author: luyuhuang
 * @brief:
 */

#include "reacter.h"
#include "reacter_epoll.h"
#include "comm.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>

static uint64_t _reacter_get_nextid(reacter_t r) {
    return r->next_eventid++;
}

static __thread int _pipefd[2];

static void _sighandler(int sig) {
    int save_errno = errno;
    
    int msg = sig;
    write(_pipefd[1], &msg, sizeof(int));

    errno = save_errno;
}

int reacter_asyn_read(reacter_t r, struct rfile *file, int32_t mtime, read_cb callback, void *data)
{
    if (hashmap_is_in(r->file_events, &file->fd))
        return -1;

    struct revent *event = (struct revent*)calloc(1, sizeof(struct revent));
    event->eventid = _reacter_get_nextid(r);
    event->r = r;
    event->fd = file->fd;
    event->type = REVENT_READ;
    event->mtime = mtime;
    event->callback = (void*)callback;
    event->data = data;

    hashmap_add(r->reacter_events, &event->eventid, event);

    struct _m_file *new_file = (struct _m_file*)calloc(1, sizeof(struct _m_file));
    new_file->eventid = event->eventid;
    new_file->fd = file->fd;

    hashmap_add(r->file_events, &new_file->fd, new_file);

    if (mtime >= 0) {
        struct _h_timer *new_timer = (struct _h_timer*)calloc(1, sizeof(struct _h_timer));
        new_timer->eventid = event->eventid;
        new_timer->absolute_mtime = get_absolute_time(mtime);
        minheap_add(r->time_heap, new_timer);
    }

    int ret = repoll_add_read_file(r->epfd, file->fd, TRUE);
    if (ret == 0)
        return REACTER_OK;
    else
        return REACTER_ERR;
}


int reacter_asyn_write(reacter_t r, struct rfile *file, void *buffer, size_t len, int32_t mtime, write_cb callback, void *data)
{
    if (hashmap_is_in(r->file_events, &file->fd))
        return -1;

    struct revent *event = (struct revent*)calloc(1, sizeof(struct revent));
    event->eventid = _reacter_get_nextid(r);
    event->r = r;
    event->fd = file->fd;
    event->type = REVENT_WRITE;
    event->mtime = mtime;
    event->buffer = buffer;
    event->buffer_len = len;
    event->callback = (void*)callback;
    event->data = data;

    hashmap_add(r->reacter_events, &event->eventid, event);

    struct _m_file *new_file = (struct _m_file*)calloc(1, sizeof(struct _m_file));
    new_file->eventid = event->eventid;
    new_file->fd = file->fd;

    hashmap_add(r->file_events, &new_file->fd, new_file);

    if (mtime >= 0) {
        struct _h_timer *new_timer = (struct _h_timer*)calloc(1, sizeof(struct _h_timer));
        new_timer->eventid = event->eventid;
        new_timer->absolute_mtime = get_absolute_time(mtime);
        minheap_add(r->time_heap, new_timer);
    }
    
    int ret = repoll_add_write_file(r->epfd, file->fd, TRUE);
    if (ret == 0)
        return REACTER_OK;
    else
        return REACTER_ERR;
}

int reacter_asyn_accept(reacter_t r, struct rfile *file, int32_t mtime, accept_cb callback, void *data)
{
    if (hashmap_is_in(r->file_events, &file->fd))
        return -1;

    struct revent *event = (struct revent*)calloc(1, sizeof(struct revent));
    event->eventid = _reacter_get_nextid(r);
    event->r = r;
    event->fd = file->fd;
    event->type = REVENT_ACCEPT;
    event->mtime = mtime;
    event->callback = (void*)callback;
    event->data = data;

    hashmap_add(r->reacter_events, &event->eventid, event);

    struct _m_file *new_file = (struct _m_file*)calloc(1, sizeof(struct _m_file));
    new_file->eventid = event->eventid;
    new_file->fd = file->fd;

    hashmap_add(r->file_events, &new_file->fd, new_file);

    if (mtime >= 0) {
        struct _h_timer *new_timer = (struct _h_timer*)calloc(1, sizeof(struct _h_timer));
        new_timer->eventid = event->eventid;
        new_timer->absolute_mtime = get_absolute_time(mtime);
        minheap_add(r->time_heap, new_timer);
    }
    
    int ret = repoll_add_read_file(r->epfd, file->fd, TRUE);
    if (ret == 0)
        return REACTER_OK;
    else
        return REACTER_ERR;
}

int reacter_asyn_connect(
    reacter_t r, struct rfile *file, struct sockaddr *addr, socklen_t len, int32_t mtime, connect_cb callback, void *data)
{
    set_nonblocking(file->fd);
    int ret = connect(file->fd, addr, len);
    if (ret == 0) {
        callback(file, file->fd, data);
        return REACTER_OK;
    } else if (ret < 0 && errno == EINPROGRESS) {
        struct revent *event = (struct revent*)calloc(1, sizeof(struct revent));
        event->eventid = _reacter_get_nextid(r);
        event->r = r;
        event->fd = file->fd;
        event->type = REVENT_CONNECT;
        event->mtime = mtime;
        event->callback = (void*)callback;
        event->data = data;

        hashmap_add(r->reacter_events, &event->eventid, event);

        struct _m_file *new_file = (struct _m_file*)calloc(1, sizeof(struct _m_file));
        new_file->eventid = event->eventid;
        new_file->fd = file->fd;

        hashmap_add(r->file_events, &new_file->fd, new_file);

        if (mtime >= 0) {
            struct _h_timer *new_timer = (struct _h_timer*)calloc(1, sizeof(struct _h_timer));
            new_timer->eventid = event->eventid;
            new_timer->absolute_mtime = get_absolute_time(mtime);
            minheap_add(r->time_heap, new_timer);
        }

        ret = repoll_add_write_file(r->epfd, file->fd, TRUE);
        if (ret == 0)
            return REACTER_OK;
        else
            return REACTER_ERR;
    } else {
        return REACTER_ERR;
    }
}

int reacter_add_timer(reacter_t r, struct rtimer *timer, timer_cb callback, void *data)
{
    if (hashmap_is_in(r->timer_events, &timer->timer_id))
        return -1;

    struct revent *event = (struct revent*)calloc(1, sizeof(struct revent));
    event->eventid = _reacter_get_nextid(r);
    event->r = r;
    event->type = REVENT_TIMER;
    event->timer_id = timer->timer_id;
    event->mtime = timer->mtime;
    event->repeat = timer->repeat;
    event->callback = (void*)callback;
    event->data = data;

    hashmap_add(r->reacter_events, &event->eventid, event);

    struct _m_timer *mtimer = (struct _m_timer*)calloc(1, sizeof(struct _m_timer));
    mtimer->eventid = event->eventid;
    mtimer->timer_id = timer->timer_id;

    hashmap_add(r->timer_events, &mtimer->timer_id, mtimer);

    struct _h_timer *new_timer = (struct _h_timer*)calloc(1, sizeof(struct _h_timer));
    new_timer->eventid = event->eventid;
    new_timer->absolute_mtime = get_absolute_time(timer->mtime);

    return minheap_add(r->time_heap, new_timer);
}

int reacter_del_timer(reacter_t r, int timer_id)
{
    if (!hashmap_is_in(r->timer_events, &timer_id))
        return -1;

    struct _m_timer *timer = (struct _m_timer*)hashmap_del(r->timer_events, &timer_id);
    struct revent *event = (struct revent*)hashmap_del(r->reacter_events, &timer->eventid);
    struct _h_timer htimer;
    htimer.eventid = event->eventid;
    struct _h_timer *ptimer = (struct _h_timer*)minheap_del(r->time_heap, &htimer);
    free(ptimer);
    free(event);
    free(timer);
    return REACTER_OK;
}

int reacter_add_signal(reacter_t r, struct rsignal *signal, signal_cb callback, void *data)
{
    if (hashmap_is_in(r->signal_events, &signal->sig))
        return -1;

    struct revent *event = (struct revent*)calloc(1, sizeof(struct revent));

    event->eventid = _reacter_get_nextid(r);
    event->r = r;
    event->sig = signal->sig;
    event->type = REVENT_SIGNAL;
    event->callback = (void*)callback;
    event->data = data;

    hashmap_add(r->reacter_events, &event->eventid, event);
    
    struct _m_signal *new_signal = (struct _m_signal*)calloc(1, sizeof(struct _m_signal));
    new_signal->eventid = event->eventid;
    new_signal->sig = signal->sig;

    hashmap_add(r->signal_events, &new_signal->sig, new_signal);

    struct sigaction sa;
    bzero(&sa, sizeof(sa));
    sa.sa_handler = _sighandler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);

    int ret = sigaction(signal->sig, &sa, NULL);
    if (ret == 0)
        return REACTER_OK;
    else
        return REACTER_ERR;
}

int reacter_del_signal(reacter_t r, int sig)
{
    if (!hashmap_is_in(r->signal_events, &signal))
        return -1;

    struct _m_signal *signal = (struct _m_signal*)hashmap_del(r->signal_events, &signal);
    struct revent *event = (struct revent*)hashmap_del(r->reacter_events, &signal->eventid);
    free(signal);
    free(event);

    struct sigaction sa;
    bzero(&sa, sizeof(sa));
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);

    int ret = sigaction(sig, &sa, NULL);
    if (ret == 0)
        return REACTER_OK;
    else
        return REACTER_ERR;
}

static int _get_signal_by_read_pipefd(int pipefd)
{
    uint8_t buffer[16];
    int ret = thorough_read(pipefd, buffer, 16);
    assert(ret == sizeof(int));
    
    int *psig = (int*)buffer;
    return *psig;
}

static struct revent *_deal_overtime_event(reacter_t r, struct _h_timer *timer)
{
    struct revent *event = hashmap_del(r->reacter_events, &timer->eventid);
    event->reason = REVENT_TIMEOUT;
    if (event->type == REVENT_ACCEPT ||
            event->type == REVENT_READ ||
            event->type == REVENT_WRITE ||
            event->type == REVENT_CONNECT) {
        struct _m_file *file = hashmap_del(r->file_events, &event->fd);
        repoll_remove_file(r->epfd, event->fd);
        free(file);
    } else if (event->type == REVENT_TIMER) {
        struct _m_timer *timer = hashmap_del(r->timer_events, &event->timer_id);
        free(timer);
    }
    return event;
}

static struct revent *_deal_signal_event(reacter_t r, int pipefd)
{
    int sig = _get_signal_by_read_pipefd(pipefd);
    struct _m_signal *signal = hashmap_get_value(r->signal_events, &sig);
    struct revent *event = hashmap_get_value(r->reacter_events, &signal->eventid);
    event->reason = REVENT_READY;
    return event;
}

static struct revent *_deal_file_event(reacter_t r, int fd)
{
    struct _m_file *file = (struct _m_file*)hashmap_del(r->file_events, &fd);
    struct revent *event = (struct revent*)hashmap_del(r->reacter_events, &file->eventid);
    event->reason = REVENT_READY;
    if (event->mtime >= 0) {
        struct _h_timer htimer;
        htimer.eventid = event->eventid;
        struct _h_timer *timer = (struct _h_timer*)minheap_del(r->time_heap, &htimer);
        free(timer);
    }
    repoll_remove_file(r->epfd, fd);
    free(file);
    return event;
}

int reacter_run(reacter_t r)
{
    repoll_event_t *evs = (repoll_event_t*)calloc(r->max_events, sizeof(repoll_event_t));
    struct _h_timer *timer;
    int32_t mtime;

    do {
        timer = (struct _h_timer*)minheap_top(r->time_heap);
        if (timer) {
            mtime = get_interval_time(timer->absolute_mtime);
            mtime = mtime > 0 ? mtime : 0;
        }
        else
            mtime = -1;

        int num_event = repoll_wait(r->epfd, evs, r->max_events, mtime);
        if (num_event < 0) {
            if (errno == EAGAIN || errno == EINTR)
                continue;
            else
                assert(num_event >= 0);
        }

        mtime = 0;
        struct revent *event;
        while (minheap_len(r->time_heap) > 0 && mtime <= 0) {
            timer = (struct _h_timer*)minheap_top(r->time_heap);
            mtime = get_interval_time(timer->absolute_mtime);
            if (mtime <= 0) {
                timer = (struct _h_timer*)minheap_pop(r->time_heap);
                event = _deal_overtime_event(r, timer);
                list_insert_at_tail(r->activity_events, event);
                free(timer);
            }
        }

        for (int i = 0; i < num_event; i++) {
            if (evs[i].repoll_events & REPOLL_IN || evs[i].repoll_events & REPOLL_OUT) {
                if (evs[i].repoll_fd == _pipefd[0]) {
                    event = _deal_signal_event(r, _pipefd[0]);
                    list_insert_at_tail(r->activity_events, event);
                } else {
                    event = _deal_file_event(r, evs[i].repoll_fd);
                    list_insert_at_tail(r->activity_events, event);
                }
            } else {

            }
        }

        list_iter_t it = list_iter_create(r->activity_events);
        while ((event = list_iter_next(it)) != NULL) {
            switch (event->type) {
                case REVENT_ACCEPT:
                    revent_on_accept(event);
                    break;
                case REVENT_CONNECT:
                    revent_on_connect(event);
                    break;
                case REVENT_READ:
                    revent_on_read(event);
                    break;
                case REVENT_WRITE:
                    revent_on_write(event);
                    break;
                case REVENT_TIMER:
                    revent_on_timer(event);
                    break;
                case REVENT_SIGNAL:
                    revent_on_signal(event);
                    break;
                default:
                    fprintf(stderr, "Bad event type\n");
                    return -1;
                    break;
            }
            event = list_del_at_head(r->activity_events);
            if (!hashmap_is_in(r->reacter_events, &event->eventid))
                free(event);
        }
        list_iter_destroy(&it);
    } while (r->loop);
    free(evs);
    return 0;
}

void reacter_stop(reacter_t r)
{
    r->loop = 0;
}

reacter_t reacter_create()
{
    return reacter_create_for_all(
        DFL_MAX_EVENTS,
        DFL_MAX_BUFFER_SIZE
    );
}

reacter_t reacter_create_for_all(
    int max_events,
    int max_buffer_size
)
{
    reacter_t reacter = (struct reacter_manager*)calloc(1, sizeof(struct reacter_manager));
    reacter->epfd = repoll_create();

    reacter->time_heap = minheap_create(_h_timer_little, _h_timer_equal);
    reacter->file_events = hashmap_create(_m_int_hash, _m_int_equal);
    reacter->signal_events = hashmap_create(_m_int_hash, _m_int_equal);
    reacter->timer_events = hashmap_create(_m_int_hash, _m_int_equal);

    reacter->reacter_events = hashmap_create(_m_uint64_hash, _m_uint64_equal);
    reacter->activity_events = list_create(_l_revent_equal);

    reacter->loop = 1;
    reacter->next_eventid = 0;

    reacter->max_events = DFL_MAX_EVENTS;
    reacter->max_buffer_size = DFL_MAX_BUFFER_SIZE;
    
    pipe(_pipefd);
    set_nonblocking(_pipefd[0]);
    repoll_add_read_file(reacter->epfd, _pipefd[0], FALSE);

    return reacter;
}

void reacter_destroy(reacter_t *r)
{
    reacter_t reacter = *r;

    struct _h_timer *timer;
    while ((timer = minheap_pop(reacter->time_heap)) != NULL) {
        free(timer);
    }
    minheap_destroy(&reacter->time_heap);

    struct hashmap_pair *pair;
    hashmap_iter_t mit = hashmap_iter_create(reacter->file_events);
    while ((pair = hashmap_iter_next(mit)) != NULL) {
        free(pair->value);
    }
    hashmap_iter_destroy(&mit);
    mit = hashmap_iter_create(reacter->signal_events);
    while ((pair = hashmap_iter_next(mit)) != NULL) {
        free(pair->value);
    }
    hashmap_iter_destroy(&mit);
    mit = hashmap_iter_create(reacter->timer_events);
    while ((pair = hashmap_iter_next(mit)) != NULL) {
        free(pair->value);
    }
    hashmap_iter_destroy(&mit);
    mit = hashmap_iter_create(reacter->reacter_events);
    while ((pair = hashmap_iter_next(mit)) != NULL) {
        free(pair->value);
    }
    hashmap_iter_destroy(&mit);

    hashmap_destroy(&reacter->file_events);
    hashmap_destroy(&reacter->signal_events);
    hashmap_destroy(&reacter->timer_events);
    hashmap_destroy(&reacter->reacter_events);
    
    struct revent *event;
    while ((event = list_del_at_head(reacter->activity_events)) != NULL) {
        free(event);
    }
    list_destroy(&reacter->activity_events);
    
    free(reacter);
}
