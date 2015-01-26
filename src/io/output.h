#ifndef IO_OUTPUT_H
#define IO_OUTPUT_H

/*
 * output variables
 */

extern struct io_output_t io_stdout;
extern struct io_output_t io_stderr;

/*
 * output function declarations
 */

struct io_output_t io_output_new(_file_t file, enum io_flag_e flags);
struct io_output_t io_output_open(const char *path, enum io_flag_e flags);

void io_output_full(struct io_output_t output, const void *restrict buf, size_t nbytes);


/**
 * Close an output.
 *   @outptut: The output.
 */

static inline void io_output_close(struct io_output_t output)
{
	output.iface->device.close(output.ref);
}

/**
 * Write to the output device.
 *   @output: The output device.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes written.
 */

static inline size_t io_output_write(struct io_output_t output, const void *restrict buf, size_t nbytes)
{
	return output.iface->write(output.ref, buf, nbytes);
}


/**
 * Output a character.
 *   @output: The output.
 *   @ch: The character.
 */

static inline void io_output_char(struct io_output_t output, char ch)
{
	io_output_full(output, &ch, sizeof(char));
}

#endif
