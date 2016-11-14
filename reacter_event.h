/**
 * @author: luyuhuang
 * @brief:
 */

#ifndef _REACTER_EVENT_H_
#define _REACTER_EVENT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <sys/time.h>

typedef struct reacter_manager *reacter_t;

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


struct _m_file {
    uint64_t eventid;
    int fd;
};

struct _m_signal {
    uint64_t eventid;
    int sig;
};

struct _m_timer {
    uint64_t eventid;
    int timer_id;
};

int _m_int_hash(void *key);
int _m_int_equal(void *key1, void *key2);

enum revent_type {
    REVENT_ACCEPT = 0,
    REVENT_CONNECT,
    REVENT_READ,
    REVENT_WRITE,
    REVENT_TIMER,
    REVENT_SIGNAL
};

enum revent_reason {
    REVENT_TIMEOUT = 0,
    REVENT_READY
};

struct revent {
    uint64_t eventid;
    enum revent_type type;
    enum revent_reason reason;

    reacter_t r;

    int fd;                 //Only used in file event
    int sig;                //Only used in signal event
    void *buffer;           //Only used in write event
    size_t buffer_len;      //Only used in write event
    int timer_id;           //Only used in timer event
    int32_t mtime;          //Only used in timer and file event;
    int repeat;             //Only used in timer event

    void *callback;
    void *data;
};

int _m_uint64_hash(void *key);
int _m_uint64_equal(void *key1, void *key2);
int _l_revent_equal(void *event1, void *event2);

struct _h_timer {
    uint64_t eventid;
    int64_t absolute_mtime;
};

int _h_timer_little(void *timer1, void *timer2);
int _h_timer_equal(void *timer1, void *timer2);

int revent_on_timer(struct revent *event);
int revent_on_signal(struct revent *event);
int revent_on_accept(struct revent *event);
int revent_on_connect(struct revent *event);
int revent_on_read(struct revent *event);
int revent_on_write(struct revent *event);

#endif //_REACTER_EVENT_H_
