#ifndef IO_CHUNK_H
#define IO_CHUNK_H

/*
 * chunk function declarations
 */

struct io_chunk_t io_chunk_str(const char *str);

size_t io_chunk_proc_len(struct io_chunk_t chunk);


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
