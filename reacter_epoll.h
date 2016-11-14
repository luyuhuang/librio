/**
 * @author: luyuhuang
 * @brief:
 */

#ifndef _REACTER_EPOLL_H_
#define _REACTER_EPOLL_H_

#include <sys/epoll.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int set_nonblocking(int fd)
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int repoll_create()
{
    return epoll_create(1024);
}

int repoll_add_read_file(int epfd, int fd, int oneshot)
{
    struct epoll_event ev;

    set_nonblocking(fd);
    ev.events = EPOLLIN | EPOLLET;
    if (oneshot)
        ev.events |= EPOLLONESHOT;
    ev.data.fd == fd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int repoll_add_write_file(int epfd, int fd, int oneshot)
{
    struct epoll_event ev;

    set_nonblocking(fd);
    ev.events = EPOLLOUT | EPOLLET;
    if (oneshot)
        ev.events |= EPOLLONESHOT;
    ev.data.fd == fd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int repoll_remove_file(int epfd, int fd)
{
    return epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
}

typedef struct epoll_event repoll_event_t;
#define repoll_fd       data.fd
#define repoll_events   events

#define REPOLL_IN       EPOLLIN
#define REPOLL_OUT      EPOLLOUT
#define REPOLL_PRI      EPOLLPRI
#define REPOLL_ERR      EPOLLERR
#define REPOLL_HUP      EPOLLHUP

int repoll_wait(int epfd, repoll_event_t *evlist, int maxevents, int timeout)
{
    return epoll_wait(epfd, evlist, maxevents, timeout);
}

#endif //_REACTER_EPOLL_H_
