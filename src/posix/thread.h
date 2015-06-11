#ifndef POSIX_THREAD_H
#define POSIX_THREAD_H

/*
 * thread function declarations
 */

void _thread_once(_once_t *once, void (*func)(void));
_thread_t _thread_new(void *(*func)(void *), void *arg);
void _thread_detach(_thread_t thread);
void *_thread_join(_thread_t thread);

/*
 * mutex function declarations
 */

_mutex_t _mutex_init();
void _mutex_destroy(_mutex_t *mutex);

void _mutex_lock(_mutex_t *mutex);
bool _mutex_trylock(_mutex_t *mutex);
void _mutex_unlock(_mutex_t *mutex);

/*
 * condition variable declarations
 */

_cond_t _cond_init();
void _cond_destroy(_cond_t *cond);

void _cond_wait(_cond_t *cond, _mutex_t *mutex);
void _cond_signal(_cond_t *cond);

/*
 * thread-local function declarations
 */

_specific_t _specific_alloc(void (*destroy)(void *));
void _specific_free(_specific_t specific);

void *_specific_get(_specific_t specific);
void _specific_set(_specific_t specific, void *ptr);


/*
 * initializer definitions
 */

#define _MUTEX_INIT PTHREAD_MUTEX_INITIALIZER

#endif
