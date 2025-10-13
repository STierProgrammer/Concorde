#ifndef __SOCKAPI_H__
#define __SOCKAPI_H__


#include <stdlib.h>
#include <stddef.h>

#ifdef _WIN32
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#endif

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#ifdef _WIN32
int sockapi_socket(int af, int type, int protocol);
SOCKET sockapi_connect(SOCKET s, const sockaddr *n, size_t nlen);
int sockapi_send(SOCKET s, const char* b, size_t blen, int f);
#endif

#ifdef __linux__
int sockapi_socket(int domain, int type, int protocol);
int sockapi_connect(int fd, const struct sockaddr* addr, socklen_t l);
int sockapi_send(int fd, const void *buf, size_t size, int f);
#endif

// TODO: lmao

#endif
