#ifndef POSIX_NET_H
#define POSIX_NET_H

/*
 * network function declarations
 */

_socket_t _socket_open(uint16_t port);
_socket_t _socket_connect(const char *host, uint16_t port);
void _socket_close(_socket_t sock);

void _socket_listen(_socket_t sock);
_socket_t _socket_accept(_socket_t sock);

void _socket_nonblock(_socket_t sock, bool enable);

size_t _socket_read(_socket_t sock, void *buf, size_t nbytes);
size_t _socket_write(_socket_t sock, const void *buf, size_t nbytes);

#endif
