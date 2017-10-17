/**
 * @author: luyuhuang
 * @brief:
 */

#ifndef _REACTER_EPOLL_H_
#define _REACTER_EPOLL_H_

#include <sys/epoll.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct epoll_event repoll_event_t;
#define repoll_fd       data.fd
#define repoll_events   events

#define REPOLL_IN       EPOLLIN
#define REPOLL_OUT      EPOLLOUT
#define REPOLL_PRI      EPOLLPRI
#define REPOLL_ERR      EPOLLERR
#define REPOLL_HUP      EPOLLHUP

int set_nonblocking(int fd);
int repoll_create();
int repoll_add_read_file(int epfd, int fd, bool oneshot);
int repoll_add_write_file(int epfd, int fd, bool oneshot);
int repoll_remove_file(int epfd, int fd);

int repoll_wait(int epfd, repoll_event_t *evlist, int maxevents, int timeout);

#endif //_REACTER_EPOLL_H_
