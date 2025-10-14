#include "sockapi.h"
#include "logging.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <stdio.h>
#include <netinet/in.h>

typedef struct {
    sock_t fd;
    char nick[MAX_NICK];
    int in_use;
} client_t;

client_t clients[MAX_CLIENTS];

// helpers
void trimnl(char *s) {
    char *p;
    if ((p = strchr(s,'\n'))) *p = 0;
    if ((p = strchr(s,'\r'))) *p = 0;
}

void broadcast(const char *msg, sock_t sender) {
    for(int i=0;i<MAX_CLIENTS;i++)
        if(clients[i].in_use && clients[i].fd!=sender)
            sockapi_send(clients[i].fd,msg,strlen(msg),0);
}

int add_client(sock_t fd) {
    for(int i=0;i<MAX_CLIENTS;i++)
        if(!clients[i].in_use){ clients[i]=(client_t){fd,"",1}; return i; }
    return -1;
}

void remove_client(int idx) {
    if(clients[idx].nick[0]){
        char msg[BUF_SIZE];
        snprintf(msg,sizeof(msg),"FROM \"__SERVER\" \"%s logged out\"\n",clients[idx].nick);
        broadcast(msg,clients[idx].fd);
    }
    sockapi_close(clients[idx].fd); clients[idx].in_use=0;
}

client_t* get_client_by_fd(sock_t fd){
    for(int i=0;i<MAX_CLIENTS;i++)
        if(clients[i].in_use && clients[i].fd==fd) return &clients[i];
    return NULL;
}

// commands
void nick_handle(client_t *c,const char *newnick){
    if(!*newnick) return;

    // Reject nicknames containing "__"
    if(strstr(newnick,"__")!=NULL){
        const char *err = "ERROR: nickname cannot contain '__'\n";
        sockapi_send(c->fd,err,strlen(err),0);
        return;
    }

    int first_time = !c->nick[0];
    strncpy(c->nick,newnick,MAX_NICK-1); c->nick[MAX_NICK-1]=0;
    if(first_time){
        char msg[BUF_SIZE];
        snprintf(msg,sizeof(msg),"FROM \"__SERVER\" \"%s logged in\"\n",c->nick);
        broadcast(msg,c->fd);
    }
}

void client_input(sock_t fd, char *buf) {
    trimnl(buf);
    client_t *c = get_client_by_fd(fd);
    if (!c) return;

    // handle NICK (todo: use an init packet that contains this if possible?)
    if (!strncmp(buf, "NICK", 4)) {
        nick_handle(c, buf + 5);
        return;
    }

    // handle SEND
    if (!strncmp(buf, "SEND ", 5)) {
        const char *content = buf + 5;

        if (*content != '"') return;
        content++; // skip opening quote

        const char *end = strchr(content, '"');
        if (!end) return;

        char message_content[BUF_SIZE];
        size_t len = end - content;
        if (len >= sizeof(message_content)) len = sizeof(message_content) - 1;
        strncpy(message_content, content, len);
        message_content[len] = '\0';

        char sender[BUF_SIZE] = "anon";
        if (c->nick[0]) strncpy(sender, c->nick, sizeof(sender) - 1);

        char msg[BUF_SIZE];
        snprintf(msg, sizeof(msg), "FROM \"%s\" \"%s\"\n", sender, message_content);
        broadcast(msg, fd);
    }
}

void accept_client(sock_t listener, fd_set *master, int *fdmax){
    struct sockaddr_in cli; socklen_t len=sizeof(cli);
    sock_t newfd = sockapi_accept(listener,(struct sockaddr*)&cli,&len);
    if(newfd<0){ server_warn(MSG_ACCEPT_FAIL); return; }

    int idx = add_client(newfd);
    if(idx<0){
        sockapi_send(newfd,MSG_SERVER_FULL,strlen(MSG_SERVER_FULL),0);
        sockapi_close(newfd);
    } else {
        FD_SET(newfd,master); if(newfd>*fdmax) *fdmax=newfd;
        sockapi_send(newfd,MSG_WELCOME,strlen(MSG_WELCOME),0);
        server_info("Client connected: fd %d",newfd);
    }
}

// main (duh)
int main(void){
    sock_t listener = sockapi_socket(AF_INET,SOCK_STREAM,0);
    if(listener<0){ server_error(MSG_SOCKET_FAIL); exit(1); }

    int opt=1; setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port   = htons(SERVER_PORT),
        .sin_addr.s_addr = INADDR_ANY,
        .sin_zero   = {0}
    };

    if(sockapi_bind(listener,(struct sockaddr*)&addr,sizeof(addr))<0 ||
       sockapi_listen(listener,5)<0){ server_error(MSG_BIND_FAIL); exit(1); }

    server_info(MSG_SERVER_START,SERVER_PORT);

    fd_set master,readfds; FD_ZERO(&master); FD_SET(listener,&master);
    int fdmax=listener; memset(clients,0,sizeof(clients));

    while(1){
        readfds=master;
        if(select(fdmax+1,&readfds,NULL,NULL,NULL)<0){ server_error(MSG_SELECT_FAIL); break; }

        for(int i=0;i<=fdmax;i++){
            if(!FD_ISSET(i,&readfds)) continue;
            if(i==listener) accept_client(listener,&master,&fdmax);
            else {
                char buf[BUF_SIZE];
                ssize_t n = sockapi_recv(i,buf,sizeof(buf)-1,0);
                if(n<=0){ client_t *c=get_client_by_fd(i); if(c) remove_client(c-clients); FD_CLR(i,&master); continue; }
                buf[n]=0; client_input(i,buf);
            }
        }
    }

    sockapi_close(listener);
    return 0;
}
