#include "../common.h"
#include "wrap.h"
#include "../mem.h"
#include "input.h"


union io_u {
	struct io_input_t input;
	struct io_output_t output;
};

/**
 * Cursor structure.
 *   @io: The I/O data.
 *   @line, col: The line and column pointers.
 */


struct cursor_t {
	union io_u io;

	uint32_t *line, *col;
};


/*
 * local function declarations
 */

static inline bool cursor_ctrl(void *ref, unsigned int cmd, void *data);
static inline size_t cursor_read(void *ref, void *restrict buf, size_t nbytes);


/**
 * Create an input cursor.
 *   @input: The input.
 *   @line: The line pointer.
 *   @col: The column pointer.
 *   &returns: The input.
 */

_export
struct io_input_t io_input_cursor(struct io_input_t input, uint32_t *line, uint32_t *col)
{
	struct cursor_t *cur;
	static const struct io_input_i iface = { { cursor_ctrl, mem_free }, cursor_read };

	cur = mem_alloc(sizeof(struct cursor_t));
	cur->io.input = input;
	cur->line = line;
	cur->col = col;

	return (struct io_input_t){ cur, &iface };
}

/**
 * Handle a control signal on the cursor.
 *   @ref: The reference.
 *   @cmd: The command.
 *   @data: The data.
 *   &returns: True if handled, false otherwise.
 */

static inline bool cursor_ctrl(void *ref, unsigned int cmd, void *data)
{
	struct cursor_t *cur = ref;

	if(cmd == io_cursor_get_e)
		*(struct io_cursor_t *)data = (struct io_cursor_t){ *cur->line, *cur->col };
	else if(cmd == io_cursor_put_e) {
		*cur->line = ((struct io_cursor_t *)data)->line;
		*cur->col = ((struct io_cursor_t *)data)->col;
	}
	else
		return false;

	return true;
}

/**
 * Read data from a cursor.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 */

static inline size_t cursor_read(void *ref, void *restrict buf, size_t nbytes)
{
	ssize_t i;
	struct cursor_t *cur = ref;

	i = nbytes = io_input_read(cur->io.input, buf, nbytes);

	while(i-- > 0) {
		if(*(char *)buf++ == '\n') {
			(*cur->line)++;
			(*cur->col) = 1;
		}
		else
			(*cur->col)++;
	}

	return nbytes;
}
