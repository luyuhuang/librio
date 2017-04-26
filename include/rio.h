/**
 * @author: luyuhuang
 * @brief:
 */

#ifndef _REACTERIO_H_
#define _REACTERIO_H_

#include <sys/socket.h>
#include <sys/types.h>

#define REACTER_OK      0
#define REACTER_EOF     0
#define REACTER_FULL    -1
#define REACTER_ERR     -2      //please check errno
#define REACTER_TIMEOUT -3

#define TRUE            1
#define FALSE           0

typedef struct reactor_manager *reactor_t;

struct rfile {
    int fd;
};

struct rtimer {
    int timer_id;
    int32_t mtime;
    int repeat;
};

struct rsignal {
    int sig;
};

typedef int (*accept_cb)(struct rfile*, int, struct sockaddr*, socklen_t, void*);
typedef int (*connect_cb)(struct rfile*, int, void*);
typedef int (*read_cb)(struct rfile*, void*, ssize_t, void*);
typedef int (*write_cb)(struct rfile*, void*, ssize_t, void*);
typedef int (*timer_cb)(struct rtimer*, void*);
typedef int (*signal_cb)(struct rsignal*, void*);


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
    int max_events;
    int max_buffer_size;
);
void reactor_destroy(reactor_t *r);

#endif //_REACTERIO_H_
