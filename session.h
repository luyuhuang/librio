/**
 * @author: luyuhuang
 * @brief: session and session manager
 */

#ifndef _SESSION_H_
#define _SESSION_H_

#include "hashmap.h"
#include "reactor_event.h"

struct session {
    struct rfile f;
    int session_id;
};
typedef struct session *session_t;

struct session_manager {
    hashmap_t session_dict;     //session_id <-> session
    int next_session_id;


};
typedef struct session_manager *session_manager_t;

session_manager_t session_manager_create();
session_manager_destroy(session_manager_t *s);

#endif //_SESSION_H_
