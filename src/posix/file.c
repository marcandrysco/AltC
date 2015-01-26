#include "../common.h"
#include "file.h"
#include <fcntl.h>


/**
 * Open a file.
 *   @path: The path.
 *   @flags: The flags.
 *   @...: The optional permissions.
 */

_export
_file_t _open(const char *path, enum _file_e flags, ...)
{
	_file_t file;

	file = open(path, O_RDONLY);

	return file;
}

/**
 * Close a file.
 *   @file: The file.
 */

_export
void _close(_file_t file)
{
	close(file);
}


/**
 * Read to a file.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to read.
 *   &returns: The number of bytes read.
 */

_export
ssize_t _read(_file_t file, void *buf, size_t nbytes)
{
	return read(file, buf, nbytes);
}

/**
 * Write to a file.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes written.
 */

_export
ssize_t _write(_file_t file, const void *buf, size_t nbytes)
{
	return write(file, buf, nbytes);
}
