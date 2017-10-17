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
server_create(const char *addr,
        newconnect_cb on_newconnect, receive_cb on_receive, connected_cb on_connected)
{
    struct server *newserver = (struct server*)malloc(sizeof(struct server));

    newserver->addr = strdup(addr);

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
    //free(server->listeners);
    free(server->addr);
    session_manager_destroy(&server->session_mgr);
}

static int
_on_read(struct rfile *file, void *buffer, ssize_t len, void *s)
{

}

static int
_on_accept(struct rfile *file, int client_fd, struct sockaddr *client_addr, socklen_t len, void *s)
{

}

int
server_listen(server_t s)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    char *strport = strchr(s->addr, ':');
    *strport = '\0';
    strport += 1;

    struct sockaddr_in server_addr;
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(strtol(strport, NULL, 10));
    inet_pton(AF_INET, s->addr, (void*)&server_addr.sin_addr);

    if (bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(fd);
        return -1;
    }
    listen(fd, 1024);

    //struct rfile *listener = s->listeners + s->listener_num++;
    s->listener.fd = fd;
    reactor_asyn_accept(REACTOR_INST, &s->listener, -1, _on_accept, s);
    return 0;
}
