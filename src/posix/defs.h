#ifndef POSIX_DEFS_H
#define POSIX_DEFS_H

/*
 * common headers
 */

#include <pthread.h>
#include <unistd.h>

/*
 * common types
 */

typedef int _file_t;
typedef int _fd_t;
typedef void *_dynlib_t;
typedef pthread_t _thread_t;
typedef pthread_key_t _specific_t;

#endif
