/**
 * @author: luyuhuang
 * @brief: common functions
 */
#ifndef _COMM_H_
#define _COMM_H_

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

int64_t get_absolute_time(int32_t mtime);
int32_t get_interval_time(int64_t mtime);

#define TREAD_EOF       0 
#define TREAD_FULL      -1
#define TREAD_ERR       -2      //please check errno

#define TWRITE_EOF      0
#define TWRITE_ERR      -2      //please check errno

ssize_t thorough_read(int fd, uint8_t *buffer, int max_size);
ssize_t thorough_write(int fd, uint8_t *buffer, int len);

#ifdef USE_MUTEX

typedef pthread_mutex_t lock_t;
#define LOCK_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define LOCK_INIT(lock) (pthread_mutex_init(lock, NULL))
#define LOCK(lock) (pthread_mutex_lock(lock))
#define UNLOCK(lock) (pthread_mutex_unlock(lock))
#define LOCK_DESTROY(lock) (pthread_mutex_destroy(lock))

#else //USE_MUTEX

typedef pthread_spinlock_t lock_t;
#define LOCK_INITIALIZER 1
#define LOCK_INIT(lock) (pthread_spin_init(lock, 0))
#define LOCK(lock) (pthread_spin_lock(lock))
#define UNLOCK(lock) (pthread_spin_unlock(lock))
#define LOCK_DESTROY(lock) (pthread_spin_destroy(lock))

#endif //USE_MUTEX


#endif //_COMM_H_
