#ifndef TYPES_DEFS_H
#define TYPES_DEFS_H

/**
 * Create callback.
 *   &returns: The pointer.
 */

typedef void *(*new_f)();

/**
 * Delete callback.
 *   @ptr: The pointer.
 */

typedef void (*delete_f)(void *ptr);

/**
 * Compare two refrences.
 *   @left: The left reference.
 *   @right: The right reference.
 *   &returns: Their order.
 */

typedef int (*compare_f)(const void *left, const void *right);

#endif
