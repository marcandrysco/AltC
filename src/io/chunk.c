#include "../common.h"
#include "chunk.h"
#include "../types/inc.h"
#include "print.h"


/*
 * local function declarations
 */

static void str_proc(struct io_output_t output, void *arg);

static bool len_ctrl(size_t *len, unsigned int cmd, void *data);
static size_t len_write(size_t *len, const void *restrict buf, size_t nbytes);


/**
 * Create a string chunk.
 *   @str: The string.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t io_chunk_str(const char *str)
{
	return (struct io_chunk_t){ str_proc, (void *)str };
}

/**
 * Processing callback for string chunks.
 *   @output: The output.
 *   @arg: the argument.
 */

static void str_proc(struct io_output_t output, void *arg)
{
	io_print_str(output, (const char *)arg);
}


/**
 * Process a chunk, retrieve the total length written.
 *   @chunk: The chunk.
 *   &returns: the total number of bytes written.
 */

_export
size_t io_chunk_proc_len(struct io_chunk_t chunk)
{
	size_t len = 0;
	static struct io_output_i iface = { { (io_ctrl_f)len_ctrl, delete_noop}, (io_write_f)len_write };
	struct io_output_t output = { &len, &iface };

	io_chunk_proc(chunk, output);

	return len;
}


/**
 * Handle a control for the length.
 *   @len: The length.
 *   @cmd: The command.
 *   @data: The data.
 *   &returns: True if handled, false otherwise.
 */

static bool len_ctrl(size_t *len, unsigned int cmd, void *data)
{
	return false;
}

/**
 * Write callback for the length.
 *   @len: The length reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes to written.
 */

static size_t len_write(size_t *len, const void *restrict buf, size_t nbytes)
{
	*len += nbytes;

	return nbytes;
}
