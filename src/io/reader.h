#ifndef IO_READER_H
#define IO_READER_H

/**
 * Reader structure.
 *   @byte: The buffered byte.
 *   @input: The input.
 *   @line, col: The line and column.
 */

struct io_reader_t {
	int16_t byte;
	struct io_input_t input;

	uint32_t line, col;
};

/*
 * reader function declarations
 */

struct io_reader_t io_reader_init(struct io_input_t input, int16_t byte);

int16_t io_reader_next(struct io_reader_t *reader);

#endif
