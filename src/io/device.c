#include "../common.h"
#include "device.h"


/**
 * No-operation for a close.
 *   @ref: The reference.
 */

void io_null_close(void *ref)
{
}

/**
 * No-operation for a close.
 *   @ref: The reference.
 *   @cmd: The command.
 *   @data: The data.
 *   &returns: Always false.
 */

bool io_null_ctrl(void *ref, unsigned int cmd, void *data)
{
	return false;
}
