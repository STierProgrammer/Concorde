#ifndef __NETAPI_H__
#define __NETAPI_H__
#include "sockapi.h"

#ifdef _WIN32
void getcrlfline(SOCKET s, char* out);
#endif 

#ifdef __linux__
void getcrlfline(int sockfd, char* out);
#endif

#endif