#ifndef __NETAPI_H__
#define __NETAPI_H__
#include "sockapi.h"
#include <stddef.h>

typedef struct NetAPIConnection {
    int connectid;
    #ifdef _WIN32
        SOCKET sock;
    #endif
    #ifdef __linux__
        int sock;
    #endif
    char* input;
    char* output;
    size_t outputtail;
    size_t outputhead;
    char* serverip;
    struct NetAPIConnection* next;
} NetAPIConnection;



NetAPIConnection* netapi_new_connection(char* serveraddr, int port);
int netapi_close_connection(NetAPIConnection* connection);
NetAPIConnection* netapi_get_connection_by_connectid(int connectid);
int netapi_send_crlf_line(NetAPIConnection* n, char* dat);
int netapi_http_send_get(NetAPIConnection* n, char* out, char* file, size_t outsz, char* dmn);
int netapi_get_data(NetAPIConnection* n, char* out, size_t s);
// hello

#endif
