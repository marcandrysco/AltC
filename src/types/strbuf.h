#ifndef TYPES_STRBUF_H
#define TYPES_STRBUF_H

/**
 * String buffer structure.
 *   @i, len: The index and length.
 *   @store: The buffer storage.
 */

struct strbuf_t {
	size_t i, len;
	char *store;
};


/*
 * string buffer function declarations
 */

struct strbuf_t strbuf_init(size_t initlen);
void strbuf_destroy(struct strbuf_t *buf);

void strbuf_reset(struct strbuf_t *buf);
void strbuf_store(struct strbuf_t *buf, char ch);
void strbuf_append(struct strbuf_t *buf, const char *restrict str);
void strbuf_write(struct strbuf_t *buf, const char *restrict data, size_t nbytes);

char *strbuf_done(struct strbuf_t *buf);
const char *strbuf_finish(struct strbuf_t *buf);

struct io_output_t strbuf_output(struct strbuf_t *buf);

#endif
