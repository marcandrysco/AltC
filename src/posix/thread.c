#include "../common.h"
#include "thread.h"

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
