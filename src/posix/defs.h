#ifndef POSIX_DEFS_H
#define POSIX_DEFS_H

/*
 * common headers
 */

#include <dirent.h>
#include <pthread.h>
#include <unistd.h>

/*
 * common types
 */

typedef int _file_t;
typedef int _fd_t;
typedef void *_dynlib_t;
typedef pthread_t _thread_t;
typedef pthread_mutex_t _mutex_t;
typedef pthread_cond_t _cond_t;
typedef pthread_key_t _specific_t;
typedef pthread_once_t _once_t;

typedef int _socket_t;

/*
 * definitions
 */

#define _ONCE_INIT PTHREAD_ONCE_INIT

#define _stdin STDIN_FILENO
#define _stdout STDOUT_FILENO
#define _stderr STDERR_FILENO


/**
 * File system iterator.
 *   @dir: The directory.
 *   @entry: The directory entry.
 */

struct _fsiter_t {
	DIR *dir;
	struct dirent entry;
};


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

#endif
