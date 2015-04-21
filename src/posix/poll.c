#include "../common.h"
#include "poll.h"
#include <poll.h>
#include "../try.h"


/**
 * Poll a set of file descriptors.
 *   @fds: THe file descriptor set.
 *   @nfds: The number of file descriptors.
 *   @timeout: The timeout. Negative values wait indefinitely.
 *   &returns: The number of signalled file descriptors.
 */

_export
unsigned int _poll(struct _poll_t *fds, unsigned int nfds, int64_t timeout)
{
	int n;
	unsigned int i;
	struct pollfd set[nfds];

	for(i = 0; i < nfds; i++) {
		set[i].fd = fds[i].fd;
		set[i].events = set[i].revents = 0;

		if(fds[i].events & _poll_in_e)
			set[i].events |= POLLIN;

		if(fds[i].events & _poll_out_e)
			set[i].events |= POLLOUT;
	}

	n = poll(set, nfds, (timeout > 0) ? (timeout / 1000) : -1);
	if(n < 0)
		throw("Poll failed. %s.", strerror(errno));

	for(i = 0; i < nfds; i++) {
		fds[i].revents = 0;

		if(set[i].revents & POLLIN)
			fds[i].revents |= _poll_in_e;

		if(set[i].revents & POLLOUT)
			fds[i].revents |= _poll_out_e;
	}

	return n;
}

/**
 * Poll a single file descriptor.
 *   @fd: The file descriptor.
 *   @events: The sought events.
 *   @timeout: The timout.
 *   &returns: The response events. Zero indicates timeout.
 */

_export
uint16_t _poll1(_fd_t fd, uint16_t events, int64_t timeout)
{
	struct _poll_t fds = { fd, events, 0 };

	_poll(&fds, 1, timeout);

	return fds.revents;
}
