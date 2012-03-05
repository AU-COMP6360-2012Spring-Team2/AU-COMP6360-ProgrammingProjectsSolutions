#ifndef H_AVOID_CONFLICT_BIND
#define H_AVOID_CONFLICT_BIND
#include <sys/socket.h>

int avoid_conflict_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    return bind(sockfd, addr, addrlen);
}

#endif
