#ifndef IO_DEFS_H
#define IO_DEFS_H


/**
 * Input/output flags.
 *   @io_none_e: No flags.
 *   @io_read_e: Read.
 *   @io_write_e: Write.
 *   @io_append_e: Append.
 *   @io_create_e: Create.
 *   @io_trunc_e: Truncate.
 *   @io_unbuf_e: Unbuffered.
 */

enum io_flag_e {
	io_none_e = 0x00,
	io_read_e = 0x01,
	io_write_e = 0x02,
	io_append_e = 0x04,
	io_create_e = 0x08,
	io_trunc_e = 0x10,
	io_unbuf_e = 0x20
};


/**
 * Control a device.
 *   @ref: The refernce.
 *   @id: The control identifier.
 *   @data: The control data.
 *   &returns: True if the signal is handle, false otherwise.
 */

typedef bool (*io_ctrl_f)(void *ref, unsigned int id, void *data);

/**
 * Close an I/O device.
 *   @ref: The reference.
 */

typedef void (*io_close_f)(void *ref);


/**
 * Device interface structure.
 *   @ctrl: Control.
 *   @close: Close.
 */

struct io_device_i {
	io_ctrl_f ctrl;
	io_close_f close;
};

/**
 * Device structure.
 *   @ref: The reference.
 *   @iface: The interface.
 */

struct io_device_t {
	void *ref;
	const struct io_device_t *iface;
};


/**
 * Output function callback.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes written.
 */

typedef size_t (*io_write_f)(void *ref, const void *restrict buf, size_t nbytes);

/**
 * Output interface.
 *   @device: The base device interface.
 *   @write: Write.
 */

struct io_output_i {
	struct io_device_i device;

	io_write_f write;
};

/**
 * Output structure.
 *   @ref: The reference.
 *   @iface: The interface.
 */

struct io_output_t {
	void *ref;
	const struct io_output_i *iface;
};


/**
 * Read function.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes read.
 */

typedef size_t (*io_read_f)(void *ref, void *restrict buf, size_t nbytes);

/**
 * Input interface.
 *   @device: The base device interface.
 *   @write: Write.
 */

struct io_input_i {
	struct io_device_i device;

	io_read_f read;
};

/**
 * Input structure.
 *   @ref: The reference.
 *   @iface: The interface.
 */

struct io_input_t {
	void *ref;
	const struct io_input_i *iface;
};


/**
 * Chunk callback.
 *   @output: The output.
 *   @arg: The argument.
 */

typedef void (*io_chunk_f)(struct io_output_t output, void *arg);

/**
 * Chunked output structure.
 *   @func: The callback function.
 *   @arg: The callback argument.
 */

struct io_chunk_t {
	io_chunk_f func;
	void *arg;
};

/**
 * Chunk creator callback.
 *   @ref: The reference.
 *   &returns: The chunk.
 */

typedef struct io_chunk_t (*io_chunk_init_f)(void *ref);


/**
 * I/O cursor structure.
 *   @line, col: The line and column.
 */

struct io_cursor_t {
	uint32_t line, col;
};


/**
 * Control signal enumerator.
 *   @io_tell_e: Tell the current position.
 *   @io_cursor_get_e: Retrieve the cursor position.
 *   @io_cursor_put_e: Set the cursor position.
 */

enum io_ctrl_e {
	io_tell_e = 0x0001,
	io_cursor_get_e = 0x0100,
	io_cursor_put_e = 0x0101,
};

#endif
