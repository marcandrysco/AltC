#include "../common.h"
#include "output.h"
#include "../io/chunk.h"
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

static size_t inst_write(struct inst_t *inst, const void *buf, size_t nbytes);
static bool inst_ctrl(struct inst_t *inst, unsigned int id, void *data);

/*
 * global variables
 */

_export struct io_output_t io_stdout;
_export struct io_output_t io_stderr;


/**
 * Create a new I/O output.
 *   @file: The file.
 *   @flags: The flags.
 */

_export
struct io_output_t io_output_new(_file_t file, enum io_flag_e flags)
{
	static const struct io_output_i iface = { { (io_ctrl_f)inst_ctrl, (io_close_f)inst_delete }, (io_write_f)inst_write };

	return (struct io_output_t){ inst_new(file, flags), &iface };
}

/**
 * Open an I/O output.
 *   @path: The path.
 *   @flags: The flags.
 */

_export
struct io_output_t io_output_open(const char *path, enum io_flag_e flags)
{
	static const struct io_output_i iface = { { (io_ctrl_f)inst_ctrl, (io_close_f)inst_close }, (io_write_f)inst_write };

	return (struct io_output_t){ inst_open(path, flags), &iface };
}


/**
 * Write an entire buffer to the output device.
 *   @output: The output device.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes.
 */

_export
void io_output_full(struct io_output_t output, const void *restrict buf, size_t nbytes)
{
	size_t rem = nbytes;

	while(rem > 0) {
		nbytes = io_output_write(output, buf, rem);
		if(nbytes == 0)
			throw("Unable to write data to output.");

		rem -= nbytes;
		buf += nbytes;
	}
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

	file = _open(path, (flags | io_write_e | io_create_e | (io_append_e ? 0 : io_trunc_e)) & ~(io_read_e));
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
 * Write to an instance.
 *   @inst: The instance.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes written.
 */

static size_t inst_write(struct inst_t *inst, const void *buf, size_t nbytes)
{
	return _write(inst->file, buf, nbytes);
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
