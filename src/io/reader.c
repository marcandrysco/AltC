#include "../common.h"
#include "reader.h"
#include "input.h"


/**
 * Initialize a reader.
 *   @input: The input.
 *   @byte: The byte.
 *   &returns: The reader.
 */

_export
struct io_reader_t io_reader_init(struct io_input_t input, int16_t byte)
{
	struct io_reader_t reader;

	reader.byte = (byte > 0) ? byte : io_input_byte(input);
	reader.input = input;
	reader.line = reader.col = 1;

	return reader;
}


/**
 * Read the next byte from the reader.
 *   @reader: The reader.
 *   &returns: The next byte.
 */

_export
int16_t io_reader_next(struct io_reader_t *reader)
{
	reader->byte = io_input_byte(reader->input);

	if(reader->byte == '\n') {
		reader->line++;
		reader->col = 1;
	}
	else
		reader->col++;

	return reader->byte;
}
