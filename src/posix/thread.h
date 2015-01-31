#ifndef POSIX_THREAD_H
#define POSIX_THREAD_H

/*
 * thread function declarations
 */

_thread_t _thread_new(void *(*func)(void *), void *arg);
void _thread_detach(_thread_t thread);
void *_thread_join(_thread_t thread);

/*
 * thread-local function declarations
 */

_specific_t _specific_alloc(void (*destroy)(void *));
void _specific_free(_specific_t specific);

void *_specific_get(_specific_t specific);
void _specific_set(_specific_t specific, void *ptr);

#endif
