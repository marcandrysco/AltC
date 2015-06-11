#include "../common.h"
#include "file.h"
#include <fcntl.h>
#include <termios.h>
#include "../try.h"


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

	if(flags & io_append_e)
		oflag |= O_APPEND;

	file = open(path, oflag, 0666);
	if(file < 0)
		throw("Failed to open '%s'. %s.", path, strerror(errno));

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
size_t _read(_file_t file, void *buf, size_t nbytes)
{
	ssize_t rd;

	rd = read(file, buf, nbytes);
	if(rd < 0)
		throw("Failed to read from file. %s.", strerror(errno));

	return rd;
}

/**
 * Write to a file.
 *   @file: The file.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes written.
 */

_export
size_t _write(_file_t file, const void *buf, size_t nbytes)
{
	ssize_t wr;
	
	wr = write(file, buf, nbytes);
	if(wr < 0)
		throw("Failed to write to file. %s.", strerror(errno));

	return wr;
}


/**
 * Flush a terminal file.
 *   @file: The file.
 */

_export
void _tcflush(_file_t file)
{
	tcflush(file, TCIFLUSH);
}
