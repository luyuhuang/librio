/**
 * @author: luyuhuang
 * @brief: common functions
 */

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>

int64_t get_absolute_time(int32_t mtime);
int32_t get_interval_time(int64_t mtime);

#define TREAD_EOF       0 
#define TREAD_FULL      -1
#define TREAD_ERR       -2      //please check errno

#define TWRITE_EOF      0
#define TWRITE_ERR      -2      //please check errno

ssize_t thorough_read(int fd, uint8_t *buffer, int max_size);
ssize_t thorough_write(int fd, uint8_t *buffer, int len);

int ip_to_sockaddr(const char *ip, int port, struct sockaddr_in *addr);
const char *sockaddr_to_ip(struct sockaddr_in *addr);
