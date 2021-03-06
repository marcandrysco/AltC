#ifndef IO_CHUNK_H
#define IO_CHUNK_H

/*
 * chunk function declarations
 */

struct io_chunk_t io_chunk_str(const char *str);
struct io_chunk_t io_chunk_space(intptr_t cnt);
struct io_chunk_t io_chunk_tab(intptr_t cnt);
struct io_chunk_t io_chunk_quote(const struct io_chunk_t *chunk);

size_t io_chunk_proc_len(struct io_chunk_t chunk);
void io_chunk_proc_str(struct io_chunk_t chunk, char *restrict str);
char *io_chunk_proc_strdup(struct io_chunk_t chunk);


#define io_chunk_quoteval(chunk) io_chunk_quote(mem_getref(struct io_chunk_t, chunk))
#define io_chunk_quotestr(str) io_chunk_quoteval(io_chunk_str(str))

/**
 * Process a chunk.
 *   @chunk: The chunk.
 *   @output: The output;
 */

static inline void io_chunk_proc(struct io_chunk_t chunk, struct io_output_t output)
{
	chunk.func(output, chunk.arg);
}

#endif
