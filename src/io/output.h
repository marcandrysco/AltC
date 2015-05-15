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
 * Control an output.
 *   @output: The output.
 *   @id: The control identifier.
 *   @data: The control data.
 *   &returns: True if the signal is handle, false otherwise.
 */

static inline bool io_output_ctrl(struct io_output_t output, unsigned int cmd, void *data)
{
	return output.iface->device.ctrl(output.ref, cmd, data);
}

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


/**
 * Retrieve the current position of the output.
 *   @output: The output.
 *   &returns: The position.
 */

static inline uint64_t io_output_tell(struct io_output_t output)
{
	uint64_t pos = 0;

	io_output_ctrl(output, io_tell_e, &pos);

	return pos;
}

#endif
