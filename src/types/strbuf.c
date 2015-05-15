#include "../common.h"
#include "strbuf.h"
#include "../mem.h"
#include "../string.h"


/*
 * local function declarations
 */

static bool output_ctrl(struct strbuf_t *buf, unsigned int id, void *data);
static void output_close(struct strbuf_t *buf);
static size_t output_write(struct strbuf_t *buf, const void *restrict ptr, size_t nbytes);


/**
 * Initialize the string buffer.
 *   @buf: The memory where the string buffer is stored.
 *   @initlen: The initial buffer length.
 *   &returns: The string buffer.
 */

_export
struct strbuf_t strbuf_init(size_t initlen)
{
	struct strbuf_t buf;

	buf.i = 0;
	buf.len = initlen;
	buf.store = mem_alloc(initlen);

	return buf;
}

/**
 * Create a new string buffer.
 *   @initlen: The initial buffer length.
 *   &returns: The string buffer.
 */

_export
struct strbuf_t *strbuf_new(size_t initlen)
{
	struct strbuf_t *buf;

	buf = mem_alloc(sizeof(struct strbuf_t));
	*buf = strbuf_init(initlen);

	return buf;
}

/**
 * Destroy a string buffer.
 *   @buf: The string buffer.
 */

_export
void strbuf_destroy(struct strbuf_t *buf)
{
	mem_free(buf->store);
}


/**
 * Reset the internal index to the beginning.
 *   @buf: The string buffer.
 */

_export
void strbuf_reset(struct strbuf_t *buf)
{
	buf->i = 0;
}

/**
 * Store a character in the string buffer.
 *   @buf: The string buffer.
 *   @ch: The character.
 */

_export
void strbuf_store(struct strbuf_t *buf, char ch)
{
	if(buf->i >= buf->len)
		buf->store = mem_realloc(buf->store, buf->len *= 2);

	buf->store[buf->i++] = ch;
}

/**
 * Append a string onto the string buffer.
 *   @buf: The string buffer.
 *   @str: The string.
 */

_export
void strbuf_append(struct strbuf_t *buf, const char *restrict str)
{
	strbuf_write(buf, str, str_len(str));
}

/**
 * Write data onto the string buffer.
 *   @buf: The string buffer.
 *   @data: The data.
 *   @nbytes: The number of bytes.
 */

_export
void strbuf_write(struct strbuf_t *buf, const char *restrict data, size_t nbytes)
{
	unsigned int i, len;

	i = buf->i + nbytes;
	if(i >= buf->len) {
		len = buf->len;

		do
			len *= 2;
		while(i >= len);

		buf->store = mem_realloc(buf->store, len);
		buf->len = len;
	}

	mem_copy(buf->store + buf->i, data, nbytes);
	buf->i = i;
}


/**
 * Write a null byte to the end of the store, returning a pointer to the
 * allocated string. This is only compatible with 'strbuf_init'.
 *   @buf: The string buffer.
 *   &returns: The buffered string.
 */

_export
char *strbuf_done(struct strbuf_t *buf)
{
	buf->store = mem_realloc(buf->store, buf->i + 1);
	buf->store[buf->i] = '\0';

	return buf->store;
}

/**
 * Write a null byte to the end of the store, return the buffered string, and
 * reset the internal index. The returned string will remain valid until the
 * next call to 'strbuf_store' or if the string buffer is destroyed.
 *   @buf: The string buffer.
 *   &returns: The buffered string.
 */

_export
const char *strbuf_finish(struct strbuf_t *buf)
{
	strbuf_store(buf, '\0');
	buf->i = 0;

	return buf->store;
}


/**
 * Create an output that writes to the string buffer.
 *   @buf: The buffer.
 *   &returns: The output.
 */

_export
struct io_output_t strbuf_output(struct strbuf_t *buf)
{
	static const struct io_output_i iface = { { (io_ctrl_f)output_ctrl, (io_close_f)output_close }, (io_write_f)output_write };

	return (struct io_output_t){ buf, &iface };
}

/**
 * Handle a control signal for a output.
 *   @buf: The buffer.
 *   @id: The control identifier.
 *   @data: The control data.
 *   &returns: True if the signal is handle, false otherwise.
 */

static bool output_ctrl(struct strbuf_t *buf, unsigned int id, void *data)
{
	if(id == io_tell_e)
		*(uint64_t *)data = buf->i;
	else
		return false;

	return true;
}

/**
 * Close the string output.
 *   @buf: The buffer.
 */

static void output_close(struct strbuf_t *buf)
{
}

/**
 * Write to an output.
 *   @buf: The buffer.
 *   @ptr: The data.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes written.
 */

static size_t output_write(struct strbuf_t *buf, const void *restrict ptr, size_t nbytes)
{
	strbuf_write(buf, ptr, nbytes);

	return nbytes;
}
