#ifndef POSIX_THREAD_H
#define POSIX_THREAD_H

/*
 * thread-local function declarations
 */

_specific_t _specific_alloc(void (*destroy)(void *));
void _specific_free(_specific_t specific);

void *_specific_get(_specific_t specific);
void _specific_set(_specific_t specific, void *ptr);

#endif
