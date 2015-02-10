#include "../common.h"
#include "fs.h"
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "../string.h"
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
 * Determine if a path references a regular file.
 *   @path: The path.
 *   &returns: True if regular file, false otherwise.
 */

_export
bool _isfile(const char *path)
{
	struct stat info;

	if(stat(path, &info))
		return false;

	return S_ISREG(info.st_mode);
}

/**
 * Determine if a path references a symbolic link.
 *   @path: The path.
 *   &returns: True if symbolic link, false otherwise.
 */

_export
bool _islink(const char *path)
{
	struct stat info;

	if(stat(path, &info))
		return false;

	return S_ISLNK(info.st_mode);
}

/**
 * Determine if a path references a directory.
 *   @path: The path.
 *   &returns: True if directory, false otherwise.
 */

_export
bool _isdir(const char *path)
{
	struct stat info;

	if(stat(path, &info))
		return false;

	return S_ISDIR(info.st_mode);
}

/**
 * Create a new directory.
 *   @path: The path.
 */

_export
void _mkdir(const char *path)
{
	if(mkdir(path, 0777) < 0)
		throw("Failed to create directory '%s'. %s.", path, strerror(errno));
}


/**
 * Remove a path, whether a directory or file.
 *   @path: The path.
 */

_export
void _remove(const char *path)
{
	if(remove(path) < 0)
		throw("Failed to remove '%s'. %s.", path, strerror(errno));
}


_export
struct _fsiter_t _fsiter_init(const char *path)
{
	DIR *dir;

	dir = opendir(path);
	if(dir == NULL)
		throw("Unable to enumerate files in '%s'. %s.", path, strerror(errno));

	return (struct _fsiter_t){ dir };
}

_export
void _fsiter_destroy(struct _fsiter_t *iter)
{
	closedir(iter->dir);
}

/**
 * Retrieve the next filename from the iterator.
 *   @iter: The iterator.
 *   &returns: The filename or null.
 */

_export
const char *_fsiter_next(struct _fsiter_t *iter)
{
	struct dirent *result;

	do
		readdir_r(iter->dir, &iter->entry, &result);
	while(result && (str_isequal(result->d_name, ".") || str_isequal(result->d_name, "..")));

	return result ? result->d_name : NULL;
}
