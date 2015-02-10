#include "../common.h"
#include "thread.h"
#include "../try.h"


/**
 * Create a new thread.
 *   @func: The function.
 *   @arg: The argument.
 *   &returns: The thread.
 */

_thread_t _thread_new(void *(*func)(void *), void *arg)
{
	int err;
	_thread_t thread;

	err = pthread_create(&thread, NULL, func, arg);
	if(err != 0)
		throw("Failed to create thread. %s.", strerror(err));

	return thread;
}

/**
 * Detach a thread.
 *   @thread: The thread.
 */

void _thread_detach(_thread_t thread)
{
	int err;

	err = pthread_detach(thread);
	if(err != 0)
		throw("Failed to detach thread. %s.", strerror(err));
}

/**
 * Join a thread.
 *   @thread: The thread.
 *   &returns: The return value.
 */

void *_thread_join(_thread_t thread)
{
	int err;
	void *ptr;

	err = pthread_join(thread, &ptr);
	if(err != 0)
		throw("Failed to join thread. %s.", strerror(err));

	return ptr;
}


/**
 * Initialize a mutex.
 *   &returns: The mutex.
 */

_mutex_t _mutex_init()
{
	int err;
	_mutex_t mutex;

	err = pthread_mutex_init(&mutex, NULL);
	if(err != 0)
		throw("Failed create mutex. %s.", strerror(err));

	return mutex;
}

/**
 * Destroy a mutex.
 *   @mutex: The mutex.
 */

void _mutex_destroy(_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_destroy(mutex);
	if(err != 0)
		throw("Failed create mutex. %s.", strerror(err));
}


/**
 * Failed to lock mutex.
 *   @mutex: The mutex.
 */

void mutex_lock(_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_lock(mutex);
	if(err != 0)
		throw("Failed lock mutex. %s.", strerror(err));
}

/**
 * Failed to unlock mutex.
 *   @mutex: The mutex.
 */

void mutex_unlock(_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_unlock(mutex);
	if(err != 0)
		throw("Failed lock mutex. %s.", strerror(err));
}


/**
 * Allocate a thread-specific variable.
 *   @destroy: Optional. The destruction callback.
 *   &returns: The thread-specific key.
 */

_specific_t _specific_alloc(void (*destroy)(void *))
{
	_specific_t specific;

	if(pthread_key_create(&specific, destroy) != 0)
		fprintf(stderr, "Failed to create thread key. %s.\n", strerror(errno)), abort();

	return specific;
}

/**
 * Free a thread-specific variable.
 *   @specific: The thread-specific key.
 */

void _specific_free(_specific_t specific)
{
	if(pthread_key_delete(specific) != 0)
		fprintf(stderr, "Failed to delete thread key. %s.\n", strerror(errno)), abort();
}


/**
 * Get a thread-specific variable.
 *   @specific: The thread-specific key.
 *   &returns: The variable pointer.
 */

void *_specific_get(_specific_t specific)
{
	return pthread_getspecific(specific);
}

/**
 * Set a thread-specific variable.
 *   @specific: The thread-specific key.
 *   @ptr: The variable pointer.
 */

void _specific_set(_specific_t specific, void *ptr)
{
	if(pthread_setspecific(specific, ptr) != 0)
		fprintf(stderr, "Failed to set thread-specific variable. %s.\n", strerror(errno)), abort();
}
