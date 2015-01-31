#include "../common.h"
#include "dir.h"
#include "../io/chunk.h"
#include "../io/print.h"


/*
 * local function declarations
 */

static void userdir_proc(struct io_output_t output, void *arg);


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
