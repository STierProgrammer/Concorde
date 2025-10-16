#ifndef __SOCKAPI_H__
#define __SOCKAPI_H__

#include <stdlib.h>
#include <stddef.h>

#ifdef _WIN32 // windows

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>

SOCKET sockapi_socket(int af, int type, int protocol);
int sockapi_connect(SOCKET s, const sockaddr *n, size_t nlen);
int sockapi_send(SOCKET s, const char* b, size_t blen, int f);
int sockapi_sendto(SOCKET s, const char* b, size_t blen, int f, const sockaddr* t, size_t tlen);
int sockapi_recv(SOCKET s, char* b, size_t len, int f);
int sockapi_recvfrom(SOCKET s, char* b, size_t len, int f, sockaddr* from, size_t slen);
int sockapi_close(SOCKET s);
#endif // windows end

#ifdef __linux__ // linux

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef int sock_t;

sock_t sockapi_socket(int domain, int type, int protocol);
int sockapi_bind(sock_t fd, const struct sockaddr *addr, socklen_t len);
int sockapi_listen(sock_t fd, int backlog);
sock_t sockapi_accept(sock_t fd, struct sockaddr *addr, socklen_t *len);
int sockapi_connect(sock_t fd, const struct sockaddr *addr, socklen_t len);
ssize_t sockapi_send(sock_t fd, const void *buf, size_t size, int flags);
ssize_t sockapi_sendto(sock_t fd, const void *buf, size_t size, int flags,
                       const struct sockaddr *dest, socklen_t len);
ssize_t sockapi_recv(sock_t fd, void *buf, size_t size, int flags);
ssize_t sockapi_recvfrom(sock_t fd, void *buf, size_t size, int flags,
                         struct sockaddr *src, socklen_t *len);
int sockapi_close(sock_t fd);
#endif // linux end

// TODO: lmao

#endif
