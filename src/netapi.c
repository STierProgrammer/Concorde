#include <netapi.h>
#include <sockapi.h>
#include <string.h>
#include <stdio.h>

int glob_connectid = 0;
NetAPIConnection* connect_head = NULL;

NetAPIConnection* netapi_new_connection(char* server_addr, int port) {
    if(!server_addr) return NULL;
    NetAPIConnection* ncon = calloc(1, sizeof(struct NetAPIConnection));
    ncon->connectid = ++glob_connectid;
    ncon->sock = sockapi_socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sock = {0};
    sock.sin_addr.s_addr = inet_addr(server_addr);
    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    ncon->serverip = server_addr;
    int slen = sizeof(sock);
    #ifdef _WIN32
    if(sockapi_connect(ncon->sock, (struct sockaddr*)&sock, slen))
        return NULL;
    #endif
    #ifdef __linux__ 
    if(sockapi_connect(ncon->sock, (struct sockaddr*)&sock, slen) == -1)
        return NULL;
    #endif

    ncon->next = connect_head;
    connect_head = ncon;

    return ncon;
}

int netapi_close_connection(NetAPIConnection* connection) {
    if(!connection) return -1;
    if(connect_head == connection) {
        connect_head = connection->next;
        sockapi_close(connection->sock);
        free(connection);
        return 0;
    }
    NetAPIConnection* curr = connect_head;
    if(!connect_head) return 0xDEAD;
    while(curr->next != connection && curr->next)
        curr = curr->next;
    if(!curr->next) return 0xBEEF;
    curr->next = curr->next->next;
    sockapi_close(connection->sock);
    free(connection);
    return 0;
}
int netapi_send_crlf_line(NetAPIConnection *n, char *dat) {
    if(!n || !dat) return -1;
    sockapi_send(n->sock, dat, strlen(dat), 0);
    sockapi_send(n->sock, "\r\n", 2, 0);
    return 0;
}

int netapi_http_send_get(NetAPIConnection* n, char *out, char* file, size_t outsz, char* dmn) {
    if(!n || !out || !file || !dmn) 
        return -1;

    char* req = malloc(1024);
    snprintf(req, 1024, "GET %s HTTP/1.1\r\nHost: %s\r\n", file, dmn);
    if(netapi_send_crlf_line(n, req) == -1)
        return -1;
    if(sockapi_recv(n->sock, out, outsz, 0) == -1)
        return -1;
    free(req);
    return 0;
}
int netapi_get_data(NetAPIConnection* n, char* out, size_t s) {
    if(!n || !out)
        return -1;
    if(sockapi_recv(n->sock, out, s, 0) == -1) 
        return -1;
    return 0;
}

int netapi_get_data_until_cr(NetAPIConnection *n, char *out) {
    if(!n || !out) return -1;
    int i = 0;
    for(;;) {
        if(sockapi_recv(n->sock, out, 1, 0) == -1) 
            return -1; 
        if(out[i] == '\r') 
            break;
        i++;
    }
    return 0;
}
int netapi_dump_connect_to_stdout(NetAPIConnection* n) {
    if(!n)
        return -1;
    printf("connectid: %d\nserverip: %s\nsock: %d\nnextptr: %p\n", n->connectid, n->serverip, n->sock, (void*)n->next);
    return 0;
}



