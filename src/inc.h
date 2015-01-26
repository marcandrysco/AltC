#ifndef INC_H
#define INC_H

/*
 * headers
 */

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * trace definitions
 */

#if _debug
#	define RES_NTRACE	8
#endif


/**
 * Resource information structure.
 *   @up: The previous information structure.
 *   @fatal: The fatal flag.
 *   @jmpbuf: The jump buffer.
 *   @memcnt, nodecnt: The memory and node count.
 *   @nbytes: The number of bytes.
 *   @mhead, mtail: Memory resource head and tail nodes.
 *   @nhead, ntail: General resource head and tail nodes.
 */

struct res_info_t {
	struct res_info_t *up;

	bool fatal;
	jmp_buf jmpbuf;

#if _debug || _test
	unsigned long memcnt, nodecnt;
	size_t nbytes;
#endif

	struct _res_mem_t *mhead, *mtail;
	struct _res_node_t *nhead, *ntail;
};

/**
 * Memory node sturcture.
 *   @prev, next: The previous and next memory nodes.
 *   @nbytes: The number of bytes.
 *   @trace: The trace.
 */

struct _res_mem_t {
	struct _res_mem_t *prev, *next;

#if _debug || _test
	size_t nbytes;
#endif

#if _debug
	void *trace[RES_NTRACE];
#endif
};

/**
 * General node structure.
 *   @prev, next: The previous and next general nodes.
 *   @destroy: The destruction callback.
 *   @trace: The trace.
 */

struct _res_node_t {
	struct _res_node_t *prev, *next;

	void (*destroy)(struct _res_node_t *);

#if _debug
	void *trace[RES_NTRACE];
#endif
};


/*
 * resource function declarations
 */

void _res_init();
void _res_destroy();

void _res_add(struct _res_mem_t *mem, size_t nbytes);
void _res_remove(struct _res_mem_t *mem);

#endif
