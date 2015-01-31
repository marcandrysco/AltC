#include "../common.h"
#include "fs.h"
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "../try.h"


/**
 * Check if a file or directory exists.
 *   @path: The path.
 *   &returns: True if exists, false otherwise.
 */

_export
bool _exists(const char *path)
{
	return (access(path, F_OK) == 0) ? true : false;
}

/**
 * Create a new directory.
 *   @path: The path.
 */

_export
void _mkdir(const char *path)
{
	if(mkdir(path, 0777) < 0)
		throw("Failed to create directory. %s.", strerror(errno));
}
