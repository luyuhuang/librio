#include "include/rio.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int pipefd[2];

static int on_timer(struct rtimer *timer, void *data)
{
    static int a = 1;
    
    if (a % 3 == 0)
        write(pipefd[1], "timer!", 8);
    if (a >= 20)
        write(pipefd[1], "exit", 5);
    printf("%s\n", (char*)data);
    a++;
    return 0;
}

static int on_signal(struct rsignal *signal, void *data)
{
    printf("SIGNAL:%d\n", signal->sig);
}

static int on_read(struct rfile *file, void *buffer, ssize_t len, void *data)
{
    char *str = (char*)buffer;
    reacter_t r = (reacter_t)data;

    if (strcmp(str, "exit") == 0)
        reacter_stop(r);
    printf("IN:%s\n", str);
    reacter_asyn_read(r, file, -1, on_read, r);
}

int main()
{
    printf("PID:%ld\n", getpid());
    reacter_t r = reacter_create();  
    struct rtimer timer;
    timer.mtime = 1000;
    timer.timer_id = 314;
    timer.repeat = TRUE;
    reacter_add_timer(r, &timer, on_timer, "tick");

    pipe(pipefd);

    struct rfile file;
    file.fd = pipefd[0];
    reacter_asyn_read(r, &file, -1, on_read, r);

    struct rsignal signal;
    signal.sig = SIGUSR1;

    reacter_add_signal(r, &signal, on_signal, NULL);

    reacter_run(r);
    reacter_destroy(&r);
    return 0;
}
