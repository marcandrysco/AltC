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
typedef pthread_key_t _specific_t;


/**
 * File system iterator.
 *   @dir: The directory.
 *   @entry: The directory entry.
 */

struct _fsiter_t {
	DIR *dir;
	struct dirent entry;
};

#endif
