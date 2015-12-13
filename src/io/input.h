#ifndef IO_INPUT_H
#define IO_INPUT_H

/*
 * input variables
 */

extern struct io_input_t io_stdin;

/*
 * input function declarations
 */

struct io_input_t io_input_new(_file_t file, enum io_flag_e flags);
struct io_input_t io_input_open(const char *path, enum io_flag_e flags);

void io_input_full(struct io_input_t input, void *restrict buf, size_t nbytes);
char *io_input_line(struct io_input_t input);


/**
 * Input a 32-bit integer.
 *   @input: The input.
 *   &returns: The value
 */

static inline int32_t io_input_int32(struct io_input_t input)
{
	int32_t val;

	io_input_full(input, &val, sizeof(int32_t));

	return val;
}

/**
 * Input an unsigned 32-bit integer.
 *   @input: The input.
 *   &returns: The value.
 */

static inline uint32_t io_input_uint32(struct io_input_t input)
{
	uint32_t val;

	io_input_full(input, &val, sizeof(uint32_t));

	return val;
}


/**
 * Control an input.
 *   @input: The input.
 *   @id: The control identifier.
 *   @data: The control data.
 *   &returns: True if the signal is handle, false otherwise.
 */

static inline bool io_input_ctrl(struct io_input_t input, unsigned int cmd, void *data)
{
	return input.iface->device.ctrl(input.ref, cmd, data);
}

/**
 * Close an input.
 *   @outptut: The input.
 */

static inline void io_input_close(struct io_input_t input)
{
	input.iface->device.close(input.ref);
}

/**
 * Write to the input device.
 *   @input: The input device.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes read.
 */

static inline size_t io_input_read(struct io_input_t input, void *restrict buf, size_t nbytes)
{
	return input.iface->read(input.ref, buf, nbytes);
}


/**
 * Input a character.
 *   @input: The input.
 *   &returns: The character.
 */

static inline char io_input_char(struct io_input_t input)
{
	char ch;

	io_input_full(input, &ch, sizeof(char));

	return ch;
}

/**
 * Read a single byte or return end-of-file indication.
 *   @input: The input.
 *   &returns: The byte or '-1' if at end-of-file.
 */

static inline int16_t io_input_byte(struct io_input_t input)
{
	size_t read;
	uint8_t byte;

	read = io_input_read(input, &byte, sizeof(uint8_t));
	return read ? byte : -1;
}


/**
 * Retrieve the cursor for an input.
 *   @input: The input.
 *   &returns: The cursor.
 */

static inline struct io_cursor_t io_input_cursor_get(struct io_input_t input)
{
	struct io_cursor_t cur = { 0, 0 };

	io_input_ctrl(input, io_cursor_get_e, &cur);

	return cur;
}

#endif
