#include "../common.h"
#include "dir.h"
#include "../io/chunk.h"
#include "../io/print.h"
#include "../posix/dir.h"


/*
 * local function declarations
 */

static void user_proc(struct io_output_t output, void *arg);


/**
 * Retrieve the applicaiton user configuration directory as a chunk.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t app_dir_user(const char *app)
{
	return (struct io_chunk_t){ user_proc, (void *)app };
}

/**
 * Process a user directory.
 *   @output: The output.
 *   @arg: Unused argument.
 */

static void user_proc(struct io_output_t output, void *arg)
{
	io_printf(output, "%C/%s", _userdirchunk(), arg);
}
