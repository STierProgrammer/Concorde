#include "sockapi.h"

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

int sockapi_bind(sock_t fd, const struct sockaddr *addr, socklen_t len) {
    return bind(fd, addr, len);
}

int sockapi_listen(sock_t fd, int backlog) {
    return listen(fd, backlog);
}

sock_t sockapi_accept(sock_t fd, struct sockaddr *addr, socklen_t *len) {
    return accept(fd, addr, len);
}

int sockapi_connect(sock_t fd, const struct sockaddr *addr, socklen_t len) {
    return connect(fd, addr, len);
}

ssize_t sockapi_send(sock_t fd, const void *buf, size_t size, int flags) {
    return send(fd, buf, size, flags);
}

ssize_t sockapi_sendto(sock_t fd, const void *buf, size_t size, int flags,
                       const struct sockaddr *dest, socklen_t len) {
    return sendto(fd, buf, size, flags, dest, len);
}

ssize_t sockapi_recv(sock_t fd, void *buf, size_t size, int flags) {
    return recv(fd, buf, size, flags);
}

ssize_t sockapi_recvfrom(sock_t fd, void *buf, size_t size, int flags,
                         struct sockaddr *src, socklen_t *len) {
    return recvfrom(fd, buf, size, flags, src, len);
}

int sockapi_close(sock_t fd) {
    return close(fd);
}

#endif
