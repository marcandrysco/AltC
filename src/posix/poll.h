#ifndef POSIX_POLL_H
#define POSIX_POLL_H

/*
 * poll function declarations
 */

unsigned int _poll(struct _poll_t *fds, unsigned int nfds, int64_t timeout);
uint16_t _poll1(_fd_t fd, uint16_t events, int64_t timeout);

#endif
