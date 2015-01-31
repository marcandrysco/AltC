#include "../common.h"
#include "file.h"
#include <fcntl.h>


/**
 * Open a file.
 *   @path: The path.
 *   @flags: The flags.
 *   @...: The optional permissions.
 *   &returns: The file.
 */

_export
_file_t _open(const char *path, enum _file_e flags, ...)
{
	_file_t file;
	int oflag;

	if(flags & io_read_e & io_write_e)
		oflag = O_RDWR;
	else if(flags & io_read_e)
		oflag = O_RDONLY;
	else
		oflag = O_WRONLY;

	if(flags & io_trunc_e)
		oflag |= O_TRUNC;

	if(flags & io_create_e)
		oflag |= O_CREAT;

	file = open(path, oflag, 0666);

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
