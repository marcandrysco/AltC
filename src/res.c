#include "common.h"
#include "res.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "posix/inc.h"


/*
 * local variables
 */

static _specific_t specific;
static _mutex_t lock;
static unsigned long memcnt, nodecnt;
#if _debug || _test
static size_t memnbytes;
#endif

/*
 * extern function declarations
 */

void _mem_release(struct _res_mem_t *mem);


/**
 * Initialize the resource tracker.
 */

void _res_init(void)
{
	lock = _mutex_init();

	memcnt = nodecnt = 0;
#if _debug || _test
	memnbytes = 0;
#endif

	specific = _specific_alloc(NULL);
	res_push();
}

/**
 * Destroy the resource tracker.
 */

void _res_destroy(void)
{
#if _test || _debug
	if(memcnt > 0)
		fprintf(stderr, "Memory leaked. Missed %lu allocations taking %zu bytes.\n", memcnt, memnbytes);

	if(nodecnt > 0)
		fprintf(stderr, "Resouces leaked. Missed %lu resources.\n", nodecnt);
#endif

	_mutex_destroy(&lock);
	res_check();
	res_pop();
	_specific_free(specific);
}


/**
 * Retrieve the current resource structure.
 *   &returns: The resource structure.
 */

_export
struct res_info_t *res_info(void)
{
	return _specific_get(specific);
}

/**
 * Check a resource structure for leaks.
 */

_export
void res_check(void)
{
	struct res_info_t *info;

	info = res_info();
	if(info == NULL)
		fprintf(stderr, "No resource structure to check.");

#if _debug
	void **func;
	struct _res_mem_t *mem;

	for(mem = info->mhead; mem != NULL; mem = mem->next) {
		fprintf(stderr, "Leaked %zu bytes.\n", mem->nbytes);

		for(func = mem->trace; *func != NULL; func++)
			fprintf(stderr, "  %p\n", *func);
	}
#endif
}

/**
 * Retrieve the current error.
 *   &returns: The error or null.
 */

_export
const char *res_error(void)
{
	struct res_info_t *info;

	info = res_info();
	return info ? info->error : NULL;
}


/**
 * Push a resource structure.
 *   &returns: The resource structure.
 */

_export
struct res_info_t *res_push(void)
{
	struct res_info_t *info;

	info = malloc(sizeof(struct res_info_t));
	info->fatal = true;
	info->up = res_info();
	info->error = NULL;
	info->mhead = info->mtail = NULL;
	info->nhead = info->ntail = NULL;

	_specific_set(specific, info);

	return info;
}

/**
 * Pop a resource structure.
 */

_export
void res_pop()
{
	struct res_info_t *info, *up;

	info = res_info();
	if(info == NULL)
		fprintf(stderr, "No resource structure to pop.");

	if(info->error != NULL)
		free(info->error);

	up = info->up;
	if(up != NULL) { 
		if(info->nhead != NULL) {
			if(up->ntail != NULL)
				up->ntail->next = info->nhead;

			info->nhead->prev = up->ntail;
			up->ntail = info->ntail;
		}

		if(info->mhead != NULL) {
			if(up->mtail != NULL)
				up->mtail->next = info->mhead;
			else
				up->mhead = info->mhead;

			info->mhead->prev = up->mtail;
			up->mtail = info->mtail;
		}
	}

	free(info);

	_specific_set(specific, up);
}


/**
 * Clear all current resources.
 */

_export
void res_clear()
{
	res_nodeclear();
	res_memclear();
}

/**
 * Clear all current memory resources.
 */

_export
void res_memclear()
{
	struct res_info_t *info;
	struct _res_mem_t *cur, *next;

	info = res_info();

	cur = info->mhead;
	while(cur != NULL) {
		next = cur->next;

		memcnt--;
#if _debug || _test
		memnbytes -= cur->nbytes;
#endif
		_mem_release(cur);

		cur = next;
	}

	info->mhead = info->mtail = NULL;
}

/**
 * Clear all current node resources.
 */

_export
void res_nodeclear()
{
	struct res_info_t *info;
	struct _res_node_t *cur, *prev;

	info = res_info();

	cur = info->ntail;
	while(cur != NULL) {
		prev = cur->prev;

		nodecnt--;
		cur->destroy((void *)cur - cur->offset);

		cur = prev;
	}

	info->nhead = info->ntail = NULL;
}


/**
 * Add a memory resource.
 *   @mem: The meory resource.
 *   @nbytes: The number of bytes.
 */

void _res_add(struct _res_mem_t *mem, size_t nbytes)
{
	struct res_info_t *info;

	info = res_info();

#if _debug
	_backtrace(mem->trace, RES_NTRACE);
#endif

	mutex_lock(&lock);
	memcnt++;
#if _debug || _test
	memnbytes += mem->nbytes = nbytes;
#endif
	mutex_unlock(&lock);

	mem->prev = info->mtail;
	mem->next = NULL;

	if(info->mtail != NULL)
		info->mtail->next = mem;
	else
		info->mhead = mem;

	info->mtail = mem;
}

/**
 * Remove a memory resource.
 *   @mem: The memory resource.
 */

void _res_remove(struct _res_mem_t *mem)
{
	struct res_info_t *info;

	mutex_lock(&lock);
	memcnt--;
#if _debug || _test
	memnbytes -= mem->nbytes;
#endif
	mutex_unlock(&lock);

	info = res_info();

	if(mem->next == NULL)
		info->mtail = mem->prev;
	else
		mem->next->prev = mem->prev;

	if(mem->prev == NULL)
		info->mhead = mem->next;
	else
		mem->prev->next = mem->next;
}


/**
 * Add a resource node.
 *   @node: The resource node.
 *   @offset: The offset.
 *   @destroy: The destruction callback.
 */

_export
void res_add(struct _res_node_t *node, ssize_t offset, void (*destroy)(void *))
{
	struct res_info_t *info;

	info = res_info();

#if _debug
	//_backtrace(node->trace, RES_NTRACE);
#endif

	mutex_lock(&lock);
	nodecnt++;
	mutex_unlock(&lock);

	node->prev = info->ntail;
	node->next = NULL;
	node->offset = offset;
	node->destroy = destroy;

	if(info->ntail != NULL)
		info->ntail->next = node;
	else
		info->nhead = node;

	info->ntail = node;
}

/**
 * Remove a resource node resource.
 *   @node: The resource node.
 */

_export
void res_remove(struct _res_node_t *node)
{
	struct res_info_t *info;

	info = res_info();

	mutex_lock(&lock);
	nodecnt--;
	mutex_unlock(&lock);

	if(node->next == NULL)
		info->ntail = node->prev;
	else
		node->next->prev = node->prev;

	if(node->prev == NULL)
		info->nhead = node->next;
	else
		node->prev->next = node->next;
}
