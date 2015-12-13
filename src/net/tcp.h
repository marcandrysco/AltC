#ifndef NET_TCP_H
#define NET_TCP_H

/*
 * structure prototypes
 */

struct tcp_client_t;

/*
 * tcp client function declarations
 */

struct tcp_client_t *tcp_client_new(_socket_t sock);
struct tcp_client_t *tcp_client_open(const char *host, uint16_t port);
void tcp_client_close(struct tcp_client_t *client);

_socket_t tcp_client_sock(struct tcp_client_t *client);
_socket_t tcp_client_events(struct tcp_client_t *client);
struct _poll_t tcp_client_poll(struct tcp_client_t *client);

void tcp_client_proc(struct tcp_client_t *client, enum _poll_e events);

size_t tcp_avail(struct tcp_client_t *client);
size_t tcp_queue(struct tcp_client_t *client);

bool tcp_ctrl(struct tcp_client_t *client, unsigned int cmd, void *data);
bool tcp_read(struct tcp_client_t *client, void *restrict buf, size_t nbytes);
void tcp_write(struct tcp_client_t *client, const void *restrict buf, size_t nbytes);
void tcp_write_str(struct tcp_client_t *client, const char *restrict str);

struct io_output_t tcp_output(struct tcp_client_t *client);
struct io_input_t tcp_input(struct tcp_client_t *client);

/*
 * server function declarations
 */

struct tcp_server_t *tcp_server_open(uint16_t port);
void tcp_server_close(struct tcp_server_t *server);

_socket_t tcp_server_sock(struct tcp_server_t *server);
_socket_t tcp_server_events(struct tcp_server_t *server);
struct _poll_t tcp_server_poll(struct tcp_server_t *server);
unsigned int tcp_server_pollall(struct tcp_server_t *server, struct _poll_t *poll);

struct tcp_client_t *tcp_server_accept(struct tcp_server_t *server);

#endif
