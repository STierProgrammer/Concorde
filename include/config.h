#ifndef CONF_H
#define CONF_H

#define SERVER_PORT      1816
#define MAX_CLIENTS      32
#define BUF_SIZE         512
#define MAX_NICK         32

#define MSG_WELCOME      "welcome to concorde\n"
#define MSG_SERVER_FULL  "server full\n"
#define MSG_ACCEPT_FAIL  "accept failed"
#define MSG_SOCKET_FAIL  "socket creation failed"
#define MSG_BIND_FAIL    "bind/listen failed"
#define MSG_SELECT_FAIL  "select failed"
#define MSG_CLIENT_LOG   "%s logged in\n"
#define MSG_CLIENT_LOGOUT "%s logged out\n"
#define MSG_SERVER_START "Server running on port %d"

#endif