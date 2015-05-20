#include "common.h"
#include "fs.h"
#include "io/inc.h"
#include "posix/inc.h"
#include "string.h"


/*
 * local function declarations
 */

static void dirname_proc(struct io_output_t output, void *arg);


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


/**
 * Retrieve a chunk for the directory name of a path.
 *   @path: The path.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t fs_dirname(const char *path)
{
	return (struct io_chunk_t){ dirname_proc, (void *)path };
}

/**
 * Retrieve the directory name length.
 *   @path: The path.
 *   &returns: The length.
 */

_export
size_t fs_dirname_len(const char *path)
{
	char *endptr;

	endptr = str_rchr(path, '/');
	return endptr ? (endptr - path) : str_len(path);
}

/**
 * Process a directory name chunk.
 *   @output: The output.
 *   @arg: The argument.
 */

static void dirname_proc(struct io_output_t output, void *arg)
{
	const char *path = arg;

	io_output_write(output, path, fs_dirname_len(path));
}
