#include "../common.h"
#include "trace.h"
#include <execinfo.h>


/**
 * Retrieve a backtrace of functions.
 *   @buf: The buffer.
 *   @n: The buffer size.
 */

void _backtrace(void **buf, unsigned int n)
{
	backtrace(buf, n-1);
}
