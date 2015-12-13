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
 * Compare characters.
 *   @left: The left characterspointer.
 *   @right: The right characterspointer.
 *   &returns: Their order.
 */

_export
int compare_char(const void *left, const void *right)
{
	if(*(char *)left > *(char *)right)
		return 1;
	else if(*(char *)left < *(char *)right)
		return -1;
	else
		return 0;
}

/**
 * Compare integers.
 *   @left: The left integer pointer.
 *   @right: The right integer pointer.
 *   &returns: Their order.
 */

_export
int compare_int(const void *left, const void *right)
{
	if(*(int *)left > *(int *)right)
		return 1;
	else if(*(int *)left < *(int *)right)
		return -1;
	else
		return 0;
}

/**
 * Compare unsigned integers.
 *   @left: The left unsigned integer pointer.
 *   @right: The right unsigned integer pointer.
 *   &returns: Their order.
 */

_export
int compare_uint(const void *left, const void *right)
{
	if(*(unsigned int *)left > *(unsigned int *)right)
		return 1;
	else if(*(unsigned int *)left < *(unsigned int *)right)
		return -1;
	else
		return 0;
}

/**
 * Compare two pointers.
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
 * Compare two strings as pointers.
 *   @left: The left pointer.
 *   @right: The right pointer.
 *   &returns: Their order.
 */

_export
int compare_str(const void *left, const void *right)
{
	return str_cmp(left, right);
}
