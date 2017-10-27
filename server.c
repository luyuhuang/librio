/**
 * @author: luyuhuang
 * @brief: server
 */

#include "reactor.h"
#include "reactor_event.h"
#include "server.h"
#include <stdlib.h>
#include <unistd.h>

server_t
server_create(newconnect_cb on_newconnect, receive_cb on_receive, connected_cb on_connected)
{
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
server_listen(const char *addr)
{
    
}

void
server_run()
{
    
}
