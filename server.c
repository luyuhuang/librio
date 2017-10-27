/**
 * @author: luyuhuang
 * @brief: server
 */

#include "reactor.h"
#include "reactor_event.h"
#include "server.h"
#include "comm.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

server_t
server_create(newconnect_cb on_newconnect, receive_cb on_receive, connected_cb on_connected)
{
    REACTOR_INST;
    struct server *newserver = (struct server*)malloc(sizeof(struct server));

    newserver->listeners = (struct rfile*)malloc(sizeof(struct rfile) * INIT_LISTERNER_LEN);
    newserver->listener_num = 0;
    newserver->listener_len = INIT_LISTERNER_LEN;

    newserver->on_newconnect = on_newconnect;
    newserver->on_receive = on_receive;
    newserver->on_connected = on_connected;

    newserver->session_mgr = session_manager_create();

    return newserver;
}

void
server_destroy(server_t *s)
{
    if (!s || !*s)
        return;

    struct server *server = *s;
    free(server->listeners);
    session_manager_destroy(&server->session_mgr);
}

int
server_listen(server_t s, const char *addr)
{
    if (s->listener_num >= s->listener_len) {
        s->listener_len *= 2;
        s->listeners = (struct rfile *)realloc(s->listeners, s->listener_len);
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    char *ip = strdup(addr);
    char *strport = strchr(ip, ':');
    *strport = '\0';
    strport += 1;

    struct sockaddr_in server_addr;
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(strtol(strport, NULL, 10));
    inet_pton(AF_INET, ip, (void*)&server_addr.sin_addr);

    if (bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(fd);
        return -1;
    }
    listen(fd, 1024);

    s->listeners[s->listener_num++].fd = fd;
    reactor_asyn_accept(REACTOR_INST, s->listeners + (s->listener_num - 1), -1, NULL, NULL);
}

void
server_run()
{
    
}
