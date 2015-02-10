#ifndef POSIX_THREAD_H
#define POSIX_THREAD_H

/*
 * thread function declarations
 */

_thread_t _thread_new(void *(*func)(void *), void *arg);
void _thread_detach(_thread_t thread);
void *_thread_join(_thread_t thread);

/*
 * mutex function declarations
 */

_mutex_t _mutex_init();
void _mutex_destroy(_mutex_t *mutex);

void mutex_lock(_mutex_t *mutex);
void mutex_unlock(_mutex_t *mutex);

/*
 * thread-local function declarations
 */

_specific_t _specific_alloc(void (*destroy)(void *));
void _specific_free(_specific_t specific);

void *_specific_get(_specific_t specific);
void _specific_set(_specific_t specific, void *ptr);

#endif
