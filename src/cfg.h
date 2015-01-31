#ifndef CFG_H
#define CFG_H

/**
 * Configuration reader.
 *   @input: The input.
 *   @depth: The depth.
 *   @line: The line number.
 */

struct cfg_reader_t {
	struct io_input_t input;
	unsigned int depth;

	unsigned long line;
};

/**
 * Configuration writer.
 *   @output: The output.
 *   @tab: The indent level.
 */

struct cfg_writer_t {
	struct io_output_t output;
	unsigned int tab;
};

/**
 * Configuration line structure.
 *   @nvalues: The number of values.
 *   @key, value: The key and values.
 */

struct cfg_line_t {
	unsigned int nvalues;
	char *key, *value[];
};


/*
 * configuration writer function declarations
 */

struct cfg_writer_t cfg_writer_init(struct io_output_t output);
void cfg_writer_destroy(struct cfg_writer_t *writer);

void cfg_write_str(struct cfg_writer_t *writer, const char *key, const char *str);
void cfg_write_ver2(struct cfg_writer_t *writer, const char *key, unsigned int maj, unsigned int min);

/*
 * configuration reader function declarations
 */

struct cfg_reader_t cfg_reader_init(struct io_input_t input);
void cfg_reader_destroy(struct cfg_reader_t *reader);

void cfg_read_ver2(struct cfg_reader_t *reader, const char *key, unsigned int *maj, unsigned int *min);

/*
 * configuration line function declarations
 */

void cfg_line_delete(struct cfg_line_t *line);

#endif
