#include "common.h"
#include "fs.h"
#include "io/chunk.h"
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
void fs_mkdir(struct io_chunk_t path)
{
	char str[io_chunk_proc_len(path) + 1];

	io_chunk_proc_str(path, str);
	_mkdir(str);
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
