#ifndef PEGASUS_SOCKET_H
#define PEGASUS_SOCKET_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef int sockfd_t;

#define AF_INET 2
#define SOCK_STREAM 1

typedef struct {
    uint16_t sin_family;
    uint16_t sin_port;
    uint32_t sin_addr;
    uint8_t sin_zero[8];
} sockaddr_in;

static inline uint16_t htons(uint16_t host) {
    return (uint16_t)((host << 8) | (host >> 8));
}

sockfd_t socket(int domain, int type, int protocol);
int connect(sockfd_t fd, const char *ip, uint16_t port);
ssize_t send(sockfd_t fd, const void *buf, size_t len);
ssize_t recv(sockfd_t fd, void *buf, size_t len, uint32_t timeout_ms);

#endif
