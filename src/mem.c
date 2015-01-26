#include "common.h"
#include <stdlib.h>
#include <string.h>
#include "mem.h"


/**
 * Allocate memory.
 *   @nbytes: The number of bytes.
 *   &returns: The allocated memory.
 */

_export
void *mem_alloc(size_t nbytes)
{
	void *ptr;
	struct _res_mem_t *mem;

	mem = ptr = malloc(nbytes + sizeof(struct _res_mem_t));
	_res_add(mem, nbytes);

	return ptr + sizeof(struct _res_mem_t);
}

/**
 * Free memory.
 *   @ptr: The pointer.
 */

_export
void mem_free(void *ptr)
{
	struct _res_mem_t *mem;

	mem = ptr -= sizeof(struct _res_mem_t);

	_res_remove(mem);
	free(ptr);
}

/**
 * Free memory if it is not null.
 *   @ptr: The pointer.
 */

_export
void mem_erase(void *ptr)
{
	if(ptr != NULL)
		mem_free(ptr);
}

/**
 * Release memory that failed to be deallocated.
 *   @mem: The memory resource.
 */

void _mem_release(struct _res_mem_t *mem)
{
	free(mem);
}


/**
 * Copy non-overlapping data.
 *   @dest: The destination.
 *   @src: The source.
 *   @nbytes: The number of bytes.
 */

_export
void mem_copy(char *restrict dest, const char *restrict src, size_t nbytes)
{
	memcpy(dest, src, nbytes);
}

/**
 * Copy possibly overlapping data.
 *   @dest: The destination.
 *   @src: The source.
 *   @nbytes: The number of bytes.
 */

_export
void mem_move(char *dest, const char *src, size_t nbytes)
{
	memmove(dest, src, nbytes);
}
