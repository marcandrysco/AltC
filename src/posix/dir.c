#include "../common.h"
#include "dir.h"
#include "../io/chunk.h"
#include "../io/print.h"
#include "../try.h"


/*
 * local function declarations
 */

static void cwd_proc(struct io_output_t output, void *arg);

static void userdir_proc(struct io_output_t output, void *arg);


/**
 * Retrieve the current working directory.
 *   &returns: The directory as a chunk.
 */

_export
struct io_chunk_t _cwd(void)
{
	return (struct io_chunk_t){ cwd_proc, NULL };
}

/**
 * Process the current working directory.
 *   @output: The output.
 *   @arg: Unused argument.
 */

static void cwd_proc(struct io_output_t output, void *arg)
{
	char *dir;

	dir = getcwd(NULL, 0);
	io_print_str(output, dir);
	free(dir);
}

/**
 * Change the current working directory.
 *   @dir: The new directory.
 */

_export
void _chdir(const char *dir)
{
	if(chdir(dir) < 0)
		throw("Failed to change working directory. %s.", strerror(errno));
}


/**
 * Retrieve the user configuration directory.
 *   @str: The destination string buffer.
 *   @nbytes: The size of the buffer in bytes.
 *   &returns: The number of bytes to write if the buffer was large enough.
 */

_export
size_t _userdir(char *str, size_t nbytes)
{
	return snprintf(str, nbytes, "%s/.config", getenv("HOME"));
}

/**
 * Retrieve the user configuration directory as a chunk.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t _userdirchunk()
{
	return (struct io_chunk_t){ userdir_proc, NULL };
}

/**
 * Process a user directory.
 *   @output: The output.
 *   @arg: Unused argument.
 */

static void userdir_proc(struct io_output_t output, void *arg)
{
	io_printf(output, "%s/.config", getenv("HOME"));
}
