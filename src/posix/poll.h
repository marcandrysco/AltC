#ifndef POSIX_POLL_H
#define POSIX_POLL_H

/**
 * Poll structure.
 *   @fd: The file descriptor.
 *   @events, revents: The sought and respond events.
 */

struct _poll_t {
	_fd_t fd;
	uint16_t events, revents;
};

/**
 * Poll flag enumerator.
 *   @_poll_in_e: Input.
 *   @_poll_out_e: Output.
 */

enum _poll_e {
	_poll_in_e = 0x01,
	_poll_out_e = 0x02
};


/*
 * poll function declarations
 */

unsigned int _poll(struct _poll_t *fds, unsigned int nfds, int64_t timeout);
uint16_t _poll1(_fd_t fd, uint16_t events, int64_t timeout);

#endif
