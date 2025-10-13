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
int sockapi_connect(SOCKET s, const sockaddr *n, size_t nlen);
#endif
// TODO: lmao

#endif
