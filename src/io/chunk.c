#include "../common.h"
#include "chunk.h"
#include "../types/inc.h"
#include "../mem.h"
#include "../string.h"
#include "device.h"
#include "output.h"
#include "print.h"


/*
 * local function declarations
 */

static void str_proc(struct io_output_t output, void *arg);
static void space_proc(struct io_output_t output, void *arg);
static void tab_proc(struct io_output_t output, void *arg);

static void quote_proc(struct io_output_t output, void *arg);
static size_t quote_write(void *ref, const void *restrict buf, size_t nbytes);

static bool len_ctrl(size_t *len, unsigned int cmd, void *data);
static size_t len_write(size_t *len, const void *restrict buf, size_t nbytes);

static bool str_ctrl(void *ref, unsigned int cmd, void *data);
static size_t str_write(void *ref, const void *restrict buf, size_t nbytes);

static bool strdup_ctrl(void *ref, unsigned int cmd, void *data);
static size_t strdup_write(void *ref, const void *restrict buf, size_t nbytes);


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
 * Create a space chunk.
 *   @cnt: The number of spaces.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t io_chunk_space(intptr_t cnt)
{
	return (struct io_chunk_t){ space_proc, (void *)cnt };
}

/**
 * Processing callback for space chunks.
 *   @output: The output.
 *   @arg: the argument.
 */

static void space_proc(struct io_output_t output, void *arg)
{
	intptr_t i;

	for(i = 0; i < (intptr_t)arg; i++)
		io_print_char(output, ' ');
}

/**
 * Create a tab chunk.
 *   @cnt: The number of tabs.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t io_chunk_tab(intptr_t cnt)
{
	return (struct io_chunk_t){ tab_proc, (void *)cnt };
}

/**
 * Processing callback for tab chunks.
 *   @output: The output.
 *   @arg: the argument.
 */

static void tab_proc(struct io_output_t output, void *arg)
{
	intptr_t i;

	for(i = 0; i < (intptr_t)arg; i++)
		io_print_char(output, '\t');
}

/**
 * Create a quoted string chunk.
 *   @chunk: The chunk.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t io_chunk_quote(const struct io_chunk_t *chunk)
{
	return (struct io_chunk_t){ quote_proc, (void *)chunk };
}

/**
 * Process a quoted output.
 *   @output: THe output.
 *   @arg: The argument.
 */

static void quote_proc(struct io_output_t output, void *arg)
{
	static struct io_output_i iface = { { io_null_ctrl, io_null_close}, (io_write_f)quote_write };
	struct io_output_t dev = { &output, &iface };
	const struct io_chunk_t *chunk = arg;

	io_chunk_proc(*chunk, dev);
}

/**
 * Write callback for the quote.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes to written.
 */

static size_t quote_write(void *ref, const void *restrict buf, size_t nbytes)
{
	struct io_output_t output = *(struct io_output_t *)ref;
	const char *ptr;

	while(true) {
		ptr = buf;
		while((*ptr != '\0') && (*ptr != '\\') && (*ptr != '"'))
			ptr++;

		io_output_write(output, buf, (const void *)ptr - buf);

		if(*ptr == '\0')
			break;
		else if(*ptr == '\\')
			io_output_write(output, "\\\\", 2);
		else if(*ptr == '"')
			io_output_write(output, "\\\"", 2);

		buf = ptr + 1;
	}

	return nbytes;
}


/**
 * Process a chunk, retrieve the total length written.
 *   @chunk: The chunk.
 *   &returns: The total number of bytes written.
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


/**
 * Process a chunk, writing data to a string.
 *   @chunk: The chunk.
 *   @str: The destination string.
 */

_export
void io_chunk_proc_str(struct io_chunk_t chunk, char *restrict str)
{
	static struct io_output_i iface = { { (io_ctrl_f)str_ctrl, delete_noop}, (io_write_f)str_write };

	io_chunk_proc(chunk, (struct io_output_t){ (void *)&str, &iface });
	*str = '\0';
}

/**
 * Handle a control for the string.
 *   @len: The length.
 *   @cmd: The command.
 *   @data: The data.
 *   &returns: True if handled, false otherwise.
 */

static bool str_ctrl(void *ref, unsigned int cmd, void *data)
{
	return false;
}

/**
 * Write callback for the string.
 *   @len: The length reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes to written.
 */

static size_t str_write(void *ref, const void *restrict buf, size_t nbytes)
{
	char **str = ref;

	mem_copy(*str, buf, nbytes);
	*str += nbytes;

	return nbytes;
}


/**
 * Process a chunk, creating an allocated string.
 *   @chunk: The chunk.
 *   &returns: The sring.
 */

_export
char *io_chunk_proc_strdup(struct io_chunk_t chunk)
{
	struct strbuf_t buf;
	static struct io_output_i iface = { { (io_ctrl_f)strdup_ctrl, delete_noop}, (io_write_f)strdup_write };

	buf = strbuf_init(32);
	io_chunk_proc(chunk, (struct io_output_t){ (void *)&buf, &iface });
	return strbuf_done(&buf);
}

/**
 * Handle a control for the string duplication.
 *   @len: The length.
 *   @cmd: The command.
 *   @data: The data.
 *   &returns: True if handled, false otherwise.
 */

static bool strdup_ctrl(void *ref, unsigned int cmd, void *data)
{
	return false;
}

/**
 * Write callback for the string duplication.
 *   @len: The length reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes to written.
 */

static size_t strdup_write(void *ref, const void *restrict buf, size_t nbytes)
{
	size_t written = nbytes;

	for(nbytes--; nbytes != SIZE_MAX; nbytes--, buf++)
		strbuf_store(ref, *(const char *)buf);

	return written;
}
