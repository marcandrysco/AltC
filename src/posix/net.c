#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include "../common.h"
#include "net.h"
#include "../mem.h"
#include "../string.h"
#include "../try.h"


/**
 * Open a server socket.
 *   @port: The port.
 *   &returns: The socket.
 */

_export
_socket_t _socket_open(uint16_t port)
{
	_socket_t sock;
	struct sockaddr_in addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		throw("Failed to bind. %s.", strerror(errno));

	mem_zero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw("Failed to bind. %s.", strerror(errno));

	return sock;
}

/**
 * Connect a socket to a client.
 *   @host: The host.
 *   @port: The port.
 *   &returns: The socket.
 */

_export
_socket_t _socket_connect(const char *host, uint16_t port)
{
	_socket_t sock;
	struct addrinfo hints, *result, *addr;
	char serv[str_lprintf("%u", port) + 1];

	str_printf(serv, "%u", port);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(host, serv, &hints, &result);

	for(addr = result; addr != NULL; addr = addr->ai_next) {
		if((sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) < 0)
			continue;

		if(connect(sock, addr->ai_addr, addr->ai_addrlen) == 0)
			break;

		close(sock);
	}

	if(addr == NULL)
		throw("Cannot connect to %s:%u.", host, port);

	freeaddrinfo(result);

	return sock;
}

/**
 * Close a socket.
 *   @sock: The socket.
 */

_export
void _socket_close(_socket_t sock)
{
	close(sock);
}


/**
 * Listen on the socket.
 *   @sock: The socket.
 */

_export
void _socket_listen(_socket_t sock)
{
	listen(sock, SOMAXCONN);
}

/**
 * Accept a client connection on a socket.
 *   @sock: The socket.
 *   &returns: The client socket.
 */

_export
_socket_t _socket_accept(_socket_t sock)
{
	socklen_t size;
	struct sockaddr_in addr;
	_socket_t client;
	
	size = sizeof(addr);
	client = accept(sock, (struct sockaddr *)&addr, &size);     

	return client;
}


/**
 * Set the non-blocking property on the socket.
 *   @sock: The socket.
 *   @enable: Flag to enable (or disable) non-blocking I/O.
 */

_export
void _socket_nonblock(_socket_t sock, bool enable)
{
	int flags;

	flags = fcntl(sock, F_GETFL, 0);
	flags = enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
	fcntl(sock, F_SETFL, flags);
}


/**
 * Read data from a socket.
 *   @sock: The socket.
 *   @buf: The destination buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes read.
 */

_export
size_t _socket_read(_socket_t sock, void *buf, size_t nbytes)
{
	ssize_t rd;

	rd = read(sock, buf, nbytes);
	if(rd < 0)
		throw("Failed to read from socket. %s.", strerror(errno));

	return rd;
}

/**
 * Write data to a socket.
 *   @sock: The socket.
 *   @buf: The destination buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes written.
 */

_export
size_t _socket_write(_socket_t sock, const void *buf, size_t nbytes)
{
	ssize_t rd;

	rd = write(sock, buf, nbytes);
	if(rd < 0)
		throw("Failed to read from socket. %s.", strerror(errno));

	return rd;
}
