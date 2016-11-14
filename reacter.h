/**
 * @author: luyuhuang
 * @brief:
 */

#ifndef _REACTER_H_
#define _REACTER_H_

#include "reacter_event.h"
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

struct reacter_manager {
    int epfd;

    minheap_t time_heap;
    hashmap_t file_events;
    hashmap_t signal_events;
    hashmap_t timer_events;

    hashmap_t reacter_events;
    list_t activity_events;

    int loop;
    uint64_t next_eventid;

    int max_events;
    int max_buffer_size;
};

typedef struct reacter_manager *reacter_t;

int reacter_asyn_accept(reacter_t r, struct rfile *file, int32_t mtime, accept_cb callback, void *data);
int reacter_asyn_connect(
    reacter_t r, struct rfile *file, struct sockaddr *addr, socklen_t len, int32_t mtime, connect_cb callback, void *data);
int reacter_asyn_read(reacter_t r, struct rfile *file, int32_t mtime, read_cb callback, void *data);
int reacter_asyn_write(reacter_t r, struct rfile *file, void *buffer, size_t len, int32_t mtime, write_cb callback, void *data);
int reacter_add_timer(reacter_t r, struct rtimer *timer, timer_cb callback, void *data);
int reacter_del_timer(reacter_t r, int timer_id);
int reacter_add_signal(reacter_t r, struct rsignal *signal, signal_cb callback, void *data);
int reacter_del_signal(reacter_t r, int sig);

int reacter_run(reacter_t r);
void reacter_stop(reacter_t r);
reacter_t reacter_create();
reacter_t reacter_create_for_all(
    int max_events;
    int max_buffer_size;
);
void reacter_destroy(reacter_t *r);

#endif //_REACTER_H_
