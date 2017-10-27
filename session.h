/**
 * @author: luyuhuang
 * @brief: session and session manager
 */

#ifndef _SESSION_H_
#define _SESSION_H_

struct session {

};
typedef struct session *session_t;

struct session_manager {

};
typedef struct session_manager *session_manager_t;

session_manager_t session_manager_create();
session_manager_destroy(session_manager_t *s);

#endif //_SESSION_H_
