#include "common.h"
#include "fs.h"
#include "io/inc.h"
#include "posix/inc.h"
#include "string.h"


/**
 * Test if a path exists.
 *   @path: The path.
 */

_export
bool fs_exists(const char *path)
{
	return _exists(path);
}


/**
 * Create a directory.
 *   @path: The path.
 */

_export
void fs_mkdir(const char *path)
{
	_mkdir(path);
}

/**
 * Create a directory and all its parents.
 *   @path: The path.
 */

_export
void fs_mkdir_parents(const char *path)
{
	size_t n, len = str_len(path);
	char *ptr, str[len + 1];

	str_copy(str, path);

	while((len > 2) && (str[len-1] == '/'))
		str[--len] = '\0';

	while(!_exists(str)) {
		ptr = str_rchr(str, '/');
		if(ptr == NULL)
			break;

		*ptr = '\0';
	}

	n = str_len(str);
	if(n == len)
		return;

	while(n < len) {
		str[n] = '/';
		n += str_len(str + n);
		_mkdir(str);
	}
}


/**
 * Remove a path, whether a directory or file.
 *   @path: The path.
 */

_export
void fs_remove(const char *path)
{
	_remove(path);
}

/**
 * Remove a path recursing through the tree.
 *   @path: The path.
 */

_export
void fs_remove_recurse(const char *path)
{
	struct _fsiter_t iter;

	if(_isdir(path)) {
		const char *file;

		iter = _fsiter_init(path);
		while((file = _fsiter_next(&iter)) != NULL) {
			char sub[str_len(path) + str_len(file) + 2];

			str_printf(sub, "%s/%s", path, file);
			fs_remove_recurse(sub);
		}
		_fsiter_destroy(&iter);
	}

	fs_remove(path);
}


/**
 * Write a formatted string to a file.
 *   @path: The path.
 *   @format: The printf-style format.
 *   @...: The printf-style arguments.
 */

_export
void fs_writef(const char *restrict path, const char *restrict format, ...)
{
	va_list args;
	struct io_output_t output;

	output = io_output_open(path, io_none_e);

	va_start(args, format);
	io_vprintf(output, format, args);
	va_end(args);

	io_output_close(output);
}
