/**
 * @author: luyuhuang
 * @brief:
 */

#ifndef _REACTER_EVENT_H_
#define _REACTER_EVENT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include "basic.h"

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

int64_t _m_int_hash(basic_value_t key);
bool _m_int_equal(basic_value_t key1, basic_value_t key2);

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

    reactor_t r;

    int fd;                 //Only used in file event
    int sig;                //Only used in signal event
    void *buffer;           //Only used in write event
    size_t buffer_len;      //Only used in write event
    int timer_id;           //Only used in timer event
    int32_t mtime;          //Only used in timer and file event;
    int repeat;             //Only used in timer event

    void *callback;
    void *data;

    struct revent *__next__;
};

int64_t _m_uint64_hash(basic_value_t key);
bool _m_uint64_equal(basic_value_t key1, basic_value_t key2);
bool _l_revent_equal(basic_value_t event1, basic_value_t event2);

struct _h_timer {
    uint64_t eventid;
    int64_t absolute_mtime;
};

bool _h_timer_little(basic_value_t timer1, basic_value_t timer2);
//int _h_timer_equal(void *timer1, void *timer2);

int revent_on_timer(struct revent *event);
int revent_on_signal(struct revent *event);
int revent_on_accept(struct revent *event);
int revent_on_connect(struct revent *event);
int revent_on_read(struct revent *event);
int revent_on_write(struct revent *event);

#endif //_REACTER_EVENT_H_
