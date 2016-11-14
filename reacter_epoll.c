/**
 * @author: luyuhuang
 * @brief:
 */

#include "reacter_epoll.h"

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
    ev.data.fd = fd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int repoll_add_write_file(int epfd, int fd, int oneshot)
{
    struct epoll_event ev;

    set_nonblocking(fd);
    ev.events = EPOLLOUT | EPOLLET;
    if (oneshot)
        ev.events |= EPOLLONESHOT;
    ev.data.fd = fd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int repoll_remove_file(int epfd, int fd)
{
    return epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
}

int repoll_wait(int epfd, repoll_event_t *evlist, int maxevents, int timeout)
{
    return epoll_wait(epfd, evlist, maxevents, timeout);
}

