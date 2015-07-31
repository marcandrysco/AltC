#include "common.h"
#include "fs.h"
#include "io/inc.h"
#include "posix/inc.h"
#include "mem.h"
#include "string.h"
#include "try.h"


/*
 * local definitions
 */

#define NAMELEN 8

/*
 * local function declarations
 */

static void tmpfill(struct io_output_t output, void *arg);

/*
 * global variables
 */

_export struct io_chunk_t fs_tmpchunk = { tmpfill, NULL };


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
 * Check if a path is a directory.
 *   @path: The path.
 *   &returns: True if a directory, false otherwise.
 */

_export
bool fs_isdir(const char *path)
{
	return _isdir(path);
}

/**
 * Check if a path is a directory.
 *   @format: The format.
 *   @...: The printf-style argument.
 *   &returns: True if a directory, false otherwise.
 */

_export
bool fs_isdirf(const char *restrict format, ...)
{
	bool ret;
	va_list args;

	va_start(args, format);
	ret = fs_isdirfv(format, args);
	va_end(args);

	return ret;
}

/**
 * Check if a path is a directory.
 *   @format: The format.
 *   @args: The printf-style argument.
 *   &returns: True if a directory, false otherwise.
 */

_export
bool fs_isdirfv(const char *restrict format, va_list args)
{
	size_t len;
	va_list copy;

	va_copy(copy, args);
	len = str_vlprintf(format, copy);
	va_end(copy);

	{
		char path[len + 1];

		str_vprintf(path, format, args);

		return fs_isdir(path);
	}
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
 * Create a directory and all its parents.
 *   @format: The format string.
 *   @...: The printf-style arguments.
 */

_export
void fs_mkdirf_parents(const char *restrict format, ...)
{
	va_list args;

	va_start(args, format);
	fs_mkdirfv_parents(format, args);
	va_end(args);
}

/**
 * Create a directory and all its parents.
 *   @format: The format string.
 *   @...: The printf-style arguments.
 */

_export
void fs_mkdirfv_parents(const char *restrict format, va_list args)
{
	va_list copy;
	size_t len;

	va_copy(copy, args);
	len = str_vlprintf(format, copy);
	va_end(copy);

	{
		char path[len+1];

		str_vprintf(path, format, args);
		fs_mkdir_parents(path);
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
		while((file = _fsiter_next(&iter, NULL)) != NULL) {
			char sub[str_len(path) + str_len(file) + 2];

			str_printf(sub, "%s/%s", path, file);
			fs_remove_recurse(sub);
		}
		_fsiter_destroy(&iter);
	}

	fs_remove(path);
}


/**
 * Create a temporary file path. The returned path may be taken by the time
 * you can write to the file.
 *   @prefix: Optional. The temporary path prefix.
 *   &returns: The allocted path.
 */

_export
char *fs_tmpname(const char *prefix)
{
	char *path;

	if(prefix == NULL)
		fatal("stub"); //prefix = _impl_fs_tmpdir();

	path = mem_alloc(str_len(prefix) + NAMELEN + 1);

	do
		str_printf(path, "%s%C", prefix, fs_tmpchunk);
	while(fs_exists(path));

	return path;
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
 * Retrieve the base name from the path.
 *   @path: The path.
 *   &returns: The base name.
 */

_export
char *fs_basename(const char *path)
{
	char *endptr;

	endptr = str_rchr(path, '/');
	return endptr ? (endptr + 1) : (char *)path;
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
	size_t i;

	i = str_len(path);
	while((i > 0) && (path[i-1] != '/'))
		i--;

	return i;
}

/**
 * Process a directory name chunk.
 *   @output: The output.
 *   @arg: The argument.
 */

static void dirname_proc(struct io_output_t output, void *arg)
{
	ssize_t i;
	const char *path = arg;

	i = str_len(path);
	while((i > 0) && (path[i-1] != '/'))
		i--;

	if(i == 0)
		io_print_str(output, "./");
	else
		io_output_write(output, path, i);
}


/**
 * Fill in a temporary name for a output chunk.
 *   @output: The output.
 *   @arg: Unused argument.
 */

static void tmpfill(struct io_output_t output, void *arg)
{
	char buf[NAMELEN];
	unsigned int i, n;

	for(i = 0; i < NAMELEN; i++) {
		n = rand() % 62;
		if(n < 10)
			n = n + '0';
		else if(n < 36)
			n = n - 10 + 'a';
		else
			n = n - 36 + 'A';

		buf[i] = n;
	}

	io_output_write(output, buf, NAMELEN);
}
