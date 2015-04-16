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
 * Reallocate memory.
 *   @ptr: The original pointer.
 *   @nbytes: The number of bytes.
 *   &returns: The new pointer.
 */

_export
void *mem_realloc(void *ptr, size_t nbytes)
{
	struct _res_mem_t *mem;

	mem = ptr -= sizeof(struct _res_mem_t);

	_res_remove(mem);
	mem = ptr = realloc(ptr, nbytes + sizeof(struct _res_mem_t));
	_res_add(mem, nbytes);

	return ptr + sizeof(struct _res_mem_t);
}

/**
 * Set a memory pointer. The destination is freed if not null.
 *   @dest: The destination pointer.
 *   @src: Optional. Consumed. The source string.
 */

_export
void _mem_set(void **dest, void *src)
{
	mem_erase(*dest);
	*dest = src;
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
 * Free a counted listed of pointers.
 *   @nptrs: The number of pointers.
 *   @ptrs: The pointers array.
 */

_export
void _mem_freelc(unsigned int nptrs, void **ptrs)
{
	while(nptrs--)
		mem_free(*ptrs++);
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
void mem_copy(void *restrict dest, const void *restrict src, size_t nbytes)
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
void mem_move(void *dest, const void *src, size_t nbytes)
{
	memmove(dest, src, nbytes);
}

/**
 * Zero the memory.
 *   @mem: The destination.
 *   @nbytes: The number of bytes.
 */

_export
void mem_zero(void *dest, size_t nbytes)
{
	memset(dest, 0x00, nbytes);
}
