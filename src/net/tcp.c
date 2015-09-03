#include "../common.h"
#include "tcp.h"
#include "../math.h"
#include "../mem.h"
#include "../string.h"
#include "../try.h"
#include "../posix/inc.h"
#include "../types/func.h"
#include "../types/list.h"


/*
 * default defintions
 */

#define DEFSIZE	(16*1024)


/**
 * Client structure.
 *   @sock: The socket.
 *   @defsize: The default read size.
 *   @in, out: The input and output data lists.
 *   @events: The pending events.
 */

struct tcp_client_t {
	_socket_t sock;

	size_t defsize;
	struct list_root_t in, out;

	unsigned int events;
};

/**
 * Server structure.
 *   @sock: The socket.
 */

struct tcp_server_t {
	_socket_t sock;
};

struct data_t {
	struct list_node_t node;

	size_t idx, len;
	uint8_t buf[];
};

/*
 * local function declarations
 */

static size_t output_proc(void *ref, const void *restrict buf, size_t nbytes);

static struct data_t *data_first(struct list_root_t *root);
static struct data_t *data_next(struct data_t *data);


/**
 * Create a client from a socket.
 *   @sock: The socket.
 *   &returns: The client.
 */

_export
struct tcp_client_t *tcp_client_new(_socket_t sock)
{
	struct tcp_client_t *client;

	client = mem_alloc(sizeof(struct tcp_client_t));
	client->sock = sock;
	client->defsize = 16*1024;
	client->in = client->out = list_root_init();

	return client;
}

/**
 * Open a client connection.
 *   @host: The host.
 *   @port: The port.
 *   &returns: The client.
 */

_export
struct tcp_client_t *tcp_client_open(const char *host, uint16_t port)
{
	return tcp_client_new(_socket_connect(host, port));
}

/**
 * Close a client connection.
 *   @client: The client.
 */

_export
void tcp_client_close(struct tcp_client_t *client)
{
	list_root_destroy(&client->in, offsetof(struct data_t, node), mem_free);
	list_root_destroy(&client->out, offsetof(struct data_t, node), mem_free);
	_socket_close(client->sock);
	mem_free(client);
}


/**
 * Retrieve the client socket.
 *   @client: The client.
 *   &returns: The socket.
 */

_export
_socket_t tcp_client_sock(struct tcp_client_t *client)
{
	return client->sock;
}

/**
 * Retrieve the client events.
 *   @client: The client.
 *   &returns: The events.
 */

_export
_socket_t tcp_client_events(struct tcp_client_t *client)
{
	return client->events;
}

/**
 * Retrieve the poll structure for a client.
 *   @client: The client.
 *   &returns: The poll structure.
 */

_export
struct _poll_t tcp_client_poll(struct tcp_client_t *client)
{
	return (struct _poll_t){ client->sock, tcp_client_events(client), 0 };
}


/**
 * Process data on a client.
 *   @client: The client.
 *   @events: The events.
 */

_export
void tcp_client_proc(struct tcp_client_t *client, enum _poll_e events)
{
	if(events & _poll_in_e) {
		struct data_t *data;

		data = mem_alloc(sizeof(struct data_t) + DEFSIZE);
		data->idx = 0;
		data->len = _socket_read(client->sock, data->buf, DEFSIZE);

		data = mem_realloc(data, sizeof(struct data_t) + data->len);
		list_root_append(&client->in, &data->node);

		client->events &= ~_poll_in_e;
	}

	if(events & _poll_out_e) {
		struct data_t *data, *next;

		for(data = data_first(&client->out); data != NULL; data = next) {
			data->idx += _socket_write(client->sock, data->buf + data->idx, data->len - data->idx);
			if(data->idx != data->len)
				break;

			next = data_next(data);
			list_root_remove(&client->out, &data->node);
			mem_free(data);
		}

		client->events &= ~_poll_out_e;
	}
}


/**
 * Retrieve the number of available bytes from a client.
 *   @client: The client.
 *   &returns: The number of bytes.
 */

_export
size_t tcp_avail(struct tcp_client_t *client)
{
	struct data_t *data;
	size_t nbytes = 0;

	for(data = data_first(&client->in); data != NULL; data = data_next(data))
		nbytes += data->len - data->idx;

	return nbytes;
}

/**
 * Retrieve the number of bytes waiting to be written for a client.
 *   @client: The client.
 *   &returns: The number of bytes.
 */

_export
size_t tcp_queue(struct tcp_client_t *client)
{
	struct data_t *data;
	size_t nbytes = 0;

	for(data = data_first(&client->out); data != NULL; data = data_next(data))
		nbytes += data->len - data->idx;

	return nbytes;
}


/**
 * Handle a control signal to the client.
 *   @client: The client.
 *   @cmd: The command.
 *   @data: The data.
 */

_export
bool tcp_ctrl(struct tcp_client_t *client, unsigned int cmd, void *data)
{
	return false;
}

/**
 * Attempt to read from the TCP connection.
 *   @client: The client.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to read.
 *   &returns: True if read, false if the request is pending.
 */

_export
bool tcp_read(struct tcp_client_t *client, void *restrict buf, size_t nbytes)
{
	size_t len;
	struct data_t *data, *next;

	if((tcp_avail(client) < nbytes) || (buf == NULL)) {
		client->events |= _poll_in_e;
		return false;
	}

	for(data = data_first(&client->in); nbytes > 0; data = next) {
		len = m_min_size(nbytes, data->len - data->idx);

		mem_copy(buf, data->buf + data->idx, len);

		nbytes -= len;
		data->idx += len;

		if(data->idx != data->len)
			break;

		next = data_next(data);
		list_root_remove(&client->in, &data->node);
		mem_free(data);
	}

	return true;
}

/**
 * Write data to a TCP connection.
 *   @client: The client.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 */

_export
void tcp_write(struct tcp_client_t *client, const void *restrict buf, size_t nbytes)
{
	struct data_t *data;

	data = mem_alloc(sizeof(struct data_t) + nbytes);
	data->idx = 0;
	data->len = nbytes;
	mem_copy(data->buf, buf, nbytes);

	client->events |= _poll_out_e;
	list_root_append(&client->out, &data->node);
}

/**
 * Write a string to the TCP connection.
 *   @client: The client.
 *   @str: The string.
 */

_export
void tcp_write_str(struct tcp_client_t *client, const char *restrict str)
{
	tcp_write(client, str, str_len(str));
}


/**
 * Create an output for the client. The output does not need to be closed.
 *   @client: The client.
 *   &returns: The output.
 */

_export
struct io_output_t tcp_output(struct tcp_client_t *client)
{
	static const struct io_output_i iface = { { (io_ctrl_f)tcp_ctrl, delete_noop }, (io_write_f)output_proc };

	return (struct io_output_t){ client, &iface };
}

/**
 * Create an input for the client. The input does not need to be closed.
 *   @client: The client.
 *   &returns: The input.
 */

_export
struct io_input_t tcp_input(struct tcp_client_t *client)
{
	static const struct io_input_i iface = { { (io_ctrl_f)tcp_ctrl, delete_noop }, (io_read_f)tcp_read };

	return (struct io_input_t){ client, &iface };
}

/**
 * Process an output request.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 */

static size_t output_proc(void *ref, const void *restrict buf, size_t nbytes)
{
	tcp_write(ref, buf, nbytes);

	return nbytes;
}


/**
 * Open a server on a port.
 *   @port: The port.
 */

_export
struct tcp_server_t *tcp_server_open(uint16_t port)
{
	_socket_t sock;
	struct tcp_server_t *server;

	sock = _socket_open(port);
	_socket_listen(sock);

	server = mem_alloc(sizeof(struct tcp_server_t));
	server->sock = sock;

	return server;
}

/**
 * Close a server.
 *   @server: The server.
 */

_export
void tcp_server_close(struct tcp_server_t *server)
{
	_socket_close(server->sock);
	mem_free(server);
}


/**
 * Retrieve the server socket.
 *   @server: The server.
 *   &returns: The socket.
 */

_export
_socket_t tcp_server_sock(struct tcp_server_t *server)
{
	return server->sock;
}

/**
 * Retrieve the server events.
 *   @server: The server.
 *   &returns: The events.
 */

_export
_socket_t tcp_server_events(struct tcp_server_t *server)
{
	return _poll_in_e;
}

/**
 * Retrieve the poll structure for a server.
 *   @server: The server.
 *   &returns: The poll structure.
 */

_export
struct _poll_t tcp_server_poll(struct tcp_server_t *server)
{
	return (struct _poll_t){ server->sock, tcp_server_events(server), 0 };
}

/**
 * Retrieve poll data for all server and client sockets.
 *   @server: The server.
 *   @poll: Optional. The poll structure. Must be large enough for all
 *     connections.
 *   &returns: The number of sockets to poll.
 */

_export
unsigned int tcp_server_pollall(struct tcp_server_t *server, struct _poll_t *poll)
{
	if(poll != NULL) {
		poll[0] = tcp_server_poll(server);
	}

	return 1;
}


/**
 * Accept a client from the server.
 *   @server: The server.
 *   &returns: The client.
 */

_export
struct tcp_client_t *tcp_server_accept(struct tcp_server_t *server)
{
	return tcp_client_new(_socket_accept(server->sock));
}


/**
 * Retrieve the first the data.
 *   @root: The root.
 *   &returns: The data.
 */

static struct data_t *data_first(struct list_root_t *root)
{
	struct list_node_t *node;

	node = root->head;
	return node ? getcontainer(node, struct data_t, node) : NULL;
}

/**
 * Retrieve the next the data.
 *   @dat: The current data.
 *   &returns: The next data.
 */

static struct data_t *data_next(struct data_t *data)
{
	struct list_node_t *node;

	node = data->node.next;
	return node ? getcontainer(node, struct data_t, node) : NULL;
}
