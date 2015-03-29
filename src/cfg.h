#ifndef CFG_H
#define CFG_H


struct cfg_doc_t {
};

/**
 * Configuration reader.
 *   @input: The input.
 *   @depth: The depth.
 *   @buf: The buffered line.
 *   @line: The line number.
 */

struct cfg_reader_t {
	struct io_input_t input;
	unsigned int depth;

	struct cfg_line_t *buf;
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
struct cfg_writer_t cfg_writer_open(const char *path);
void cfg_writer_destroy(struct cfg_writer_t *writer);
void cfg_writer_close(struct cfg_writer_t *writer);

void cfg_begin(struct cfg_writer_t *writer, const char *key);
void cfg_end(struct cfg_writer_t *writer, const char *key);

void cfg_write(struct cfg_writer_t *writer, const char *key);
void cfg_writef(struct cfg_writer_t *writer, const char *key, const char *format, ...);
void cfg_write_str(struct cfg_writer_t *writer, const char *key, const char *str);
void cfg_write_ver2(struct cfg_writer_t *writer, const char *key, unsigned int maj, unsigned int min);

/*
 * configuration reader function declarations
 */

struct cfg_reader_t cfg_reader_init(struct io_input_t input);
struct cfg_reader_t cfg_reader_open(const char *path);
void cfg_reader_destroy(struct cfg_reader_t *reader);
void cfg_reader_close(struct cfg_reader_t *reader);

const char *cfg_reader_peak(struct cfg_reader_t *reader);
struct cfg_line_t *cfg_reader_check(struct cfg_reader_t *reader, const char *key);
struct cfg_line_t *cfg_reader_get(struct cfg_reader_t *reader, const char *key);

bool cfg_read(struct cfg_reader_t *reader, const char *restrict key);
bool cfg_readf(struct cfg_reader_t *reader, const char *restrict key, const char *restrict format, ...);
struct cfg_line_t *cfg_read_line(struct cfg_reader_t *reader);
void cfg_read_ver2(struct cfg_reader_t *reader, const char *key, unsigned int *maj, unsigned int *min);
char *cfg_read_str(struct cfg_reader_t *reader, const char *key);

/*
 * configuration line function declarations
 */

void cfg_line_delete(struct cfg_line_t *line);

#endif
