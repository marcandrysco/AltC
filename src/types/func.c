#include "../common.h"
#include "func.h"
#include "../string.h"


/**
 * Always null new function.
 *   &returns: Always null.
 */

_export
void *new_null()
{
	return NULL;
}

/**
 * No-operation deletion.
 *   @ref: The reference.
 */

_export
void delete_noop(void *ref)
{
}


/**
 * Compares two pointers.
 *   @left: The left pointer.
 *   @right: The right pointer.
 *   &returns: Their order.
 */

_export
int compare_ptr(const void *left, const void *right)
{
	if(left > right)
		return 1;
	else if(left < right)
		return -1;
	else
		return 0;
}

/**
 * Compares two strings as pointers.
 *   @left: The left pointer.
 *   @right: The right pointer.
 *   &returns: Their order.
 */

_export
int compare_str(const void *left, const void *right)
{
	return str_cmp(left, right);
}
