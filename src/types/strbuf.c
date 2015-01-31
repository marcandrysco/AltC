#include "../common.h"
#include "strbuf.h"
#include "../mem.h"
#include "../string.h"


/**
 * Initialize the string buffer.
 *   @buf: The memory where the string buffer is stored.
 *   @initlen: The initial buffer length.
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
