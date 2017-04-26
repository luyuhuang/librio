#include "../include/rio.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

int pipefd[2];

static int on_timer(struct rtimer *timer, void *data)
{
    static int a = 1;
    
    if (a % 3 == 0)
        assert(write(pipefd[1], "timer!", 8) == 8);
    if (a >= 10)
        assert(write(pipefd[1], "exit", 5) == 5);
    printf("%s\n", (char*)data);
    a++;
    return 0;
}

static int on_signal(struct rsignal *signal, void *data)
{
    printf("SIGNAL:%d\n", signal->sig);
    return 0;
}

static int on_read(struct rfile *file, void *buffer, ssize_t len, void *data)
{
    char *str = (char*)buffer;
    reactor_t r = (reactor_t)data;

    if (strcmp(str, "exit") == 0)
        reactor_stop(r);
    printf("IN:%s\n", str);
    reactor_asyn_read(r, file, -1, on_read, r);

    return 0;
}

int main()
{
    printf("PID:%d\n", getpid());
    reactor_t r = reactor_create();  
    assert(r);
    struct rtimer timer;
    timer.mtime = 10;
    timer.timer_id = 314;
    timer.repeat = TRUE;
    assert(reactor_add_timer(r, &timer, on_timer, "tick") == REACTER_OK);

    assert(pipe(pipefd) == 0);

    struct rfile file;
    file.fd = pipefd[0];
    assert(reactor_asyn_read(r, &file, -1, on_read, r) == REACTER_OK);

    struct rsignal signal;
    signal.sig = SIGUSR1;

    assert(reactor_add_signal(r, &signal, on_signal, NULL) == REACTER_OK);

    reactor_run(r);
    reactor_destroy(&r);
    return 0;
}
