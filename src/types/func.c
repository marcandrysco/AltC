#include "../common.h"
#include "func.h"


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
