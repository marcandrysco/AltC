#include "../common.h"
#include "input.h"
#include "../posix/inc.h"
#include "../mem.h"
#include "../try.h"


/**
 * File instance structure.
 *   @file: The raw file.
 *   @i, nbytes: The buffer index and size.
 *   @buf: The buffer.
 */

struct inst_t {
	_file_t file;

	size_t i, nbytes;
	uint8_t *buf;
};


/*
 * local function declarations
 */

static struct inst_t *inst_new(_file_t file, enum io_flag_e flags);
static struct inst_t *inst_open(const char *path, enum io_flag_e flags);
static void inst_delete(struct inst_t *inst);
static void inst_close(struct inst_t *inst);

static size_t inst_read(struct inst_t *inst, void *buf, size_t nbytes);
static bool inst_ctrl(struct inst_t *inst, unsigned int id, void *data);


/**
 * Create a new I/O input.
 *   @file: The file.
 *   @flags: The flags.
 */

_export
struct io_input_t io_input_new(_file_t file, enum io_flag_e flags)
{
	static const struct io_input_i iface = { { (io_ctrl_f)inst_ctrl, (io_close_f)inst_delete }, (io_read_f)inst_read };

	return (struct io_input_t){ inst_new(file, flags), &iface };
}

/**
 * Open an I/O input.
 *   @path: The path.
 *   @flags: The flags.
 */

_export
struct io_input_t io_input_open(const char *path, enum io_flag_e flags)
{
	static const struct io_input_i iface = { { (io_ctrl_f)inst_ctrl, (io_close_f)inst_close }, (io_read_f)inst_read };

	return (struct io_input_t){ inst_open(path, flags), &iface };
}


/**
 * Read an entire buffer from the output device.
 *   @input: The input device.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 */

_export
void io_input_full(struct io_input_t input, void *restrict buf, size_t nbytes)
{
	size_t rem = nbytes;

	while(rem > 0) {
		nbytes = io_input_read(input, buf, rem);
		if(nbytes == 0)
			throw("Unable to read data from input.");

		rem -= nbytes;
		buf += nbytes;
	}
}

/**
 * Read a line from the input.
 *   @input: The input.
 *   &returns: The line as an allocated string or null at end-of-file.
 */

_export
char *io_input_line(struct io_input_t input)
{
	char *line;
	size_t idx, len;
	int16_t byte;

	byte = io_input_byte(input);
	if(byte < 0)
		return NULL;

	idx = 0;
	line = mem_alloc(len = 16);

	do {
		if(idx == len)
			line = mem_realloc(line, len *= 2);

		line[idx++] = byte;

		if(byte == '\n')
			break;
	} while((byte = io_input_byte(input)) >= 0);

	line = mem_realloc(line, idx+1);
	line[idx] = '\0';

	return line;
}


/**
 * Create a new output instance.
 *   @file: The internal file.
 *   @flags: The flags.
 */

static struct inst_t *inst_new(_file_t file, enum io_flag_e flags)
{
	struct inst_t *inst;

	inst = mem_alloc(sizeof(struct inst_t));
	inst->file = file;

	return inst;
}

/**
 * Create a new output instance.
 *   @path: The file path.
 *   @flags: The flags.
 */

static struct inst_t *inst_open(const char *path, enum io_flag_e flags)
{
	_file_t file;

	file = _open(path, (flags | io_read_e) & ~(io_write_e | io_append_e | io_trunc_e | io_create_e));
	if(file < 0)
		throw("Cannot open file '%s'. %s.", path, _errstr);

	return inst_new(file, flags);
}

/**
 * Delete an output instance.
 *   @inst: The instance.
 */

static void inst_delete(struct inst_t *inst)
{
	mem_free(inst);
}

/**
 * Delete an output instance.
 *   @inst: The instance.
 */

static void inst_close(struct inst_t *inst)
{
	_close(inst->file);
	inst_delete(inst);
}


/**
 * Read from an instance.
 *   @inst: The instance.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to read.
 *   &returns: The number of bytes read.
 */

static size_t inst_read(struct inst_t *inst, void *buf, size_t nbytes)
{
	return _read(inst->file, buf, nbytes);
}

/**
 * Handle a control signal.
 *   @inst: The instance.
 *   @id: The identifier.
 *   @data: The data.
 *   &returns: True if the signal is handle, false otherwise.
 */

static bool inst_ctrl(struct inst_t *inst, unsigned int id, void *data)
{
	return false;
}
