#include "../../include/sockapi.h"

#ifdef _WIN32

SOCKET sockapi_socket(int af, int type, int protocol) {
    return socket(af, type, protocol);
}

int sockapi_connect(SOCKET s, const sockaddr *n, size_t nlen) {
    return connect(s, n, nlen);
}

int sockapi_send(SOCKET s, const char* b, size_t blen, int f) {
    return send(
        s,
        b,
        blen,
        f
    );
}

int sockapi_sendto(SOCKET s, const char* b, size_t blen, int f, const sockaddr* t, size_t tlen) {
    return sendto(
        s,
        b,
        blen,
        f,
        t,
        tlen
    );
}
int sockapi_recv(SOCKET s, char* b, size_t len, int f) {
    return recv(
        s,
        b,
        len,
        f
    );
}

int sockapi_recvfrom(SOCKET s, char* b, size_t len, int f, sockaddr* from, size_t slen) {
    return recvfrom(
        s,
        b,
        len,
        from,
        slen
    );
}
#endif

#ifdef __linux__

int sockapi_socket(int domain, int type, int protocol) {
    return socket(domain, type, protocol);
}

int sockapi_connect(int fd, const struct sockaddr* addr, socklen_t l) {
    return connect(
        fd,
        addr,
        l
    );
}

ssize_t sockapi_send(int fd, const void *buf, size_t size, int f) {
    return send(
        fd,
        buf,
        size,
        f
    );
}

ssize_t sockapi_sendto(int fd, const void *buf, size_t size, int f, const struct sockaddr* d, socklen_t l) {
    return sendto(
        fd,
        buf,
        size,
        f,
        d,
        l
    );
}

ssize_t sockapi_recv(int fd, void* buf, size_t size, int f) {
    return recv(
        fd,
        buf,
        size,
        f
    );
}

ssize_t sockapi_recvfrom(int fd, void* buf, size_t size, int f, struct sockaddr* addr, socklen_t* l) {
    return recvfrom(
        fd,
        buf,
        size,
        f,
        addr,
        l
    );
}

#endif
