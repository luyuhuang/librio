/**
 * @author: luyuhuang
 * @brief: common functions
 */

#include "comm.h"
#include <errno.h>
#include <unistd.h>


int64_t get_absolute_time(int32_t mtime)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t now = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return now + mtime;
}

int32_t get_interval_time(int64_t mtime)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t now = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return mtime - now;
}

ssize_t thorough_read(int fd, uint8_t *buffer, int max_size)
{
    ssize_t ret = 0;
    ssize_t nread = 0;
    ssize_t size;
    
    do {
        size = read(fd, buffer + nread, max_size - nread);
        if (size > 0) {
            nread += size;
            if (nread > max_size) {
                nread = 0;
                ret = TREAD_FULL;
            }
        } else if (size == 0) {
            ret = TREAD_EOF;
            break;
        } else if (size < 0 && errno != EINTR) {
            if (errno == EAGAIN) {
                ret = nread;
                break;
            } else {
                ret = TREAD_ERR;
                break;
            }
        }
    } while (1);

    return ret;
}

ssize_t thorough_write(int fd, uint8_t *buffer, int len)
{
    ssize_t ret = 0;
    ssize_t nwrite = 0;
    ssize_t size;

    do {
        size = write(fd, buffer + nwrite, len - nwrite);
        if (size > 0) {
            nwrite += size;
        } else if (size == 0) {
            ret = TWRITE_EOF;
            break;
        } else if (size < 0 && errno != EINTR) {
            if (errno == EAGAIN) {
                ret = nwrite;
                break;
            } else {
                ret = TWRITE_ERR;
                break;
            }
        }
    } while (nwrite < len);
    ret = nwrite;

    return ret;
}

int ip_to_sockaddr(const char *ip, int port, struct sockaddr_in *addr)
{
    addr->sin_family = PF_INET;
    int ret = inet_pton(PF_INET, ip, &addr->sin_addr);
}

const char *sockaddr_to_ip(struct sockaddr_in *addr)
{
    
}
