/**
 * @author: luyuhuang
 * @brief:
 */

#ifndef _REACTER_H_
#define _REACTER_H_

#include "reactor_event.h"
#include "minheap.h"
#include "list.h"
#include "hashmap.h"

#define DFL_MAX_EVENTS 2048
#define DFL_MAX_BUFFER_SIZE 4096

#define REACTER_OK      0
#define REACTER_EOF     0
#define REACTER_FULL    -1
#define REACTER_ERR     -2      //please check errno
#define REACTER_TIMEOUT -3

#define TRUE            1
#define FALSE           0

struct reactor_manager {
    int epfd;

    minheap_t time_heap;
    hashmap_t file_events;
    hashmap_t signal_events;
    hashmap_t timer_events;

    hashmap_t reactor_events;
    list_t activity_events;

    int loop;
    uint64_t next_eventid;

    int max_events;
    int max_buffer_size;
};

typedef struct reactor_manager *reactor_t;

int reactor_asyn_accept(reactor_t r, struct rfile *file, int32_t mtime, accept_cb callback, void *data);
int reactor_asyn_connect(
    reactor_t r, struct rfile *file, struct sockaddr *addr, socklen_t len, int32_t mtime, connect_cb callback, void *data);
int reactor_asyn_read(reactor_t r, struct rfile *file, int32_t mtime, read_cb callback, void *data);
int reactor_asyn_write(reactor_t r, struct rfile *file, void *buffer, size_t len, int32_t mtime, write_cb callback, void *data);
int reactor_add_timer(reactor_t r, struct rtimer *timer, timer_cb callback, void *data);
int reactor_del_timer(reactor_t r, int timer_id);
int reactor_add_signal(reactor_t r, struct rsignal *signal, signal_cb callback, void *data);
int reactor_del_signal(reactor_t r, int sig);

int reactor_run(reactor_t r);
void reactor_stop(reactor_t r);
reactor_t reactor_create();
reactor_t reactor_create_for_all(
    int max_events,
    int max_buffer_size
);
void reactor_destroy(reactor_t *r);

#endif //_REACTER_H_
