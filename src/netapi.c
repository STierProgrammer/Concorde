#include <netapi.h>
#include <sockapi.h>

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