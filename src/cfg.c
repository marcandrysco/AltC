#include "common.h"
#include "cfg.h"
#include "mem.h"
#include "string.h"
#include "try.h"
#include "io/inc.h"


/*
 * local function declarations
 */

static struct cfg_line_t *buf_line(struct cfg_reader_t *reader);


/**
 * Create a new configuration writer.
 *   @output: The output.
 *   &returns: The writer.
 */

_export
struct cfg_writer_t cfg_writer_init(struct io_output_t output)
{
	struct cfg_writer_t writer;

	writer.output = output;
	writer.tab = 0;

	return writer;
}

/**
 * Open a configuration writer. 
 *   @path: The path.
 *   &returns: The writer.
 */

_export
struct cfg_writer_t cfg_writer_open(const char *path)
{
	return cfg_writer_init(io_output_open(path, io_none_e));
}

/**
 * Open and allocate a configuration writer. 
 *   @path: The path.
 *   &returns: The writer.
 */

_export
struct cfg_writer_t *cfg_writer_new(const char *path)
{
	struct cfg_writer_t *writer;

	writer = mem_alloc(sizeof(struct cfg_writer_t));
	*writer = cfg_writer_open(path);

	return writer;
}

/**
 * Delete a configuration writer.
 *   @writer: The writer.
 */

_export
void cfg_writer_destroy(struct cfg_writer_t *writer)
{
	if(writer->tab != 0)
		throw("Attempting to finish writing within a tab.");
}

/**
 * Close a writer.
 *   @writer: The writer.
 */

_export
void cfg_writer_close(struct cfg_writer_t *writer)
{
	cfg_writer_destroy(writer);
	io_output_close(writer->output);
}

/**
 * Delete a writer.
 *   @writer: The writer.
 */

_export
void cfg_writer_delete(struct cfg_writer_t *writer)
{
	cfg_writer_close(writer);
	mem_free(writer);
}


/**
 * Write a begin key.
 *   @writer: The writer.
 *   @key: The key.
 */

_export
void cfg_begin(struct cfg_writer_t *writer, const char *key)
{
	io_printf(writer->output, "%C%s\n", io_chunk_tab(writer->tab), key);
	writer->tab++;
}

/**
 * Write a formatted begin line to the writer.
 *   @writer: The writer.
 *   @key: The key.
 *   @format: The format string.
 *   @...: The arguments.
 */

_export
void cfg_beginf(struct cfg_writer_t *writer, const char *key, const char *format, ...)
{
	va_list args;

	va_start(args, format);
	cfg_writefv(writer, key, format, args);
	va_end(args);

	writer->tab++;
}

/**
 * Write an end key.
 *   @writer: The writer.
 *   @key: The key.
 */

_export
void cfg_end(struct cfg_writer_t *writer, const char *key)
{
	writer->tab--;
	io_printf(writer->output, "%C%s\n", io_chunk_tab(writer->tab), key);
}


/**
 * Write a key.
 *   @writer: The writer.
 *   @key: The key.
 */

_export
void cfg_write(struct cfg_writer_t *writer, const char *key)
{
	io_printf(writer->output, "%C%s\n", io_chunk_tab(writer->tab), key);
}

/**
 * Write a formatted line to the writer.
 *   @writer: The writer.
 *   @key: The key.
 *   @format: The format string.
 *   @...: The arguments.
 */

_export
void cfg_writef(struct cfg_writer_t *writer, const char *key, const char *format, ...)
{
	va_list args;

	va_start(args, format);
	cfg_writefv(writer, key, format, args);
	va_end(args);
}

/**
 * Write a formatted line to the writer with a variable argument list.
 *   @writer: The writer.
 *   @key: The key.
 *   @format: The format string.
 *   @...: The arguments.
 */

_export
void cfg_writefv(struct cfg_writer_t *writer, const char *key, const char *format, va_list args)
{
	char space = '\t';

	io_printf(writer->output, "%C%s", io_chunk_tab(writer->tab), key);

	while(*format != '\0') {
		io_printf(writer->output, "%c", space);
		space = ' ';

		switch(*format++) {
		case 'u': io_printf(writer->output, "\"%u\"", va_arg(args, unsigned int)); break;
		case 's': io_printf(writer->output, "\"%s\"", va_arg(args, const char *)); break;
		case 'b': io_printf(writer->output, "\"%s\"", va_arg(args, int) ? "true" : "false"); break;
		case 'f': io_printf(writer->output, "\"%g\"", va_arg(args, double)); break;
		case 'C': io_printf(writer->output, "\"%C\"", va_arg(args, struct io_chunk_t)); break;
		default: throw("Invalid format character '%c'.", format[-1]);
		}
	}

	io_printf(writer->output, "\n");
}

/**
 * Write a string to the writer.
 *   @writer: The writer.
 *   @key: The key.
 *   @str: The string.
 */

_export
void cfg_write_str(struct cfg_writer_t *writer, const char *key, const char *str)
{
	io_printf(writer->output, "%C%s\t\"%s\"\n", io_chunk_tab(writer->tab), key, str);
}

/**
 * Write a 2-piece version to the writer.
 *   @writer: The writer.
 *   @key: The key.
 *   @maj: The major component.
 *   @min: The minor component.
 */

_export
void cfg_write_ver2(struct cfg_writer_t *writer, const char *key, unsigned int maj, unsigned int min)
{
	io_printf(writer->output, "%C%s\t\"%u.%u\"\n", io_chunk_tab(writer->tab), key, maj, min);
}


/**
 * Initialize a configuration reader.
 *   @input: The input.
 *   &returns: The reader.
 */

_export
struct cfg_reader_t cfg_reader_init(struct io_input_t input)
{
	struct cfg_reader_t reader;

	reader.input = input;
	reader.depth = 0;
	reader.line = 0l;
	reader.buf = buf_line(&reader);

	return reader;
}

/**
 * Open a configuration reader.
 *   @path: The path.
 *   &returns: The reader.
 */

_export
struct cfg_reader_t cfg_reader_open(const char *path)
{
	struct io_input_t input = io_input_open(path, io_none_e);

	return cfg_reader_init(input);
}

/**
 * Destroy a configuration reader.
 *   @reader: The reader.
 */

_export
void cfg_reader_destroy(struct cfg_reader_t *reader)
{
	if(reader->buf != NULL)
		cfg_line_delete(reader->buf);
}

/**
 * Close a configuration reader.
 *   @reader: The reader.
 */

_export
void cfg_reader_close(struct cfg_reader_t *reader)
{
	cfg_reader_destroy(reader);
	io_input_close(reader->input);
}


/**
 * Peak at the next key in a reader.
 *   @reader: The reader.
 *   &returns: The key or null.
 */

_export
const char *cfg_reader_peak(struct cfg_reader_t *reader)
{
	return reader->buf ? reader->buf->key : NULL;
}

/**
 * Check if the current line matches the key.
 *   @reader: The reader.
 *   @key: The key.
 *   &returns: The line if the key matches, null otherwise.
 */

_export
struct cfg_line_t *cfg_reader_check(struct cfg_reader_t *reader, const char *key)
{
	return str_chk(cfg_reader_peak(reader), key) ? reader->buf : NULL;
}

/**
 * Get the next line only if it matches the key. The next line will be
 * buffered given a match. The returned line must be deleted.
 *   @reader: The reader.
 *   @key: The key.
 *   &returns: The line if the key matches, null otherwise.
 */

_export
struct cfg_line_t *cfg_reader_get(struct cfg_reader_t *reader, const char *key)
{
	struct cfg_line_t *line;

	if(!str_chk(cfg_reader_peak(reader), key))
		return NULL;

	line = reader->buf;
	reader->buf = buf_line(reader);

	return line;
}


/**
 * Check if a line exists, throwing an error if it does not.
 *   @reader: The reader.
 *   @key: The key.
 */

_export
void cfg_check(struct cfg_reader_t *reader, const char *key)
{
	if(!cfg_read(reader, key))
		throw("Missing directive '%s'; line %u.", key, (unsigned int)reader->line);
}


/**
 * Read a blank line from the reader.
 *   @reader: The reader.
 *   @key: The key.
 *   &returns: True if line read, false otherwise.
 */

_export
bool cfg_read(struct cfg_reader_t *reader, const char *restrict key)
{
	struct cfg_line_t *line;

	line = cfg_reader_get(reader, key);
	if(line == NULL)
		return false;

	cfg_line_delete(line);

	return true;
}

/**
 * Read a formatted line from the reader.
 *   @reader: The reader.
 *   @key: The key.
 *   @format: The format string.
 *   @...: The arguments.
 *   &returns: True if line read, false otherwise.
 */

_export
bool cfg_readf(struct cfg_reader_t *reader, const char *restrict key, const char *restrict format, ...)
{
	va_list args;
	struct cfg_line_t *line;
	unsigned int i = 0;

	line = cfg_reader_get(reader, key);
	if(line == NULL)
		return false;

	va_start(args, format);

	while(*format != '\0') {
		switch(*format++) {
		case 's': *va_arg(args, char **) = str_dup(line->value[i++]); break;
		case 'u': *va_arg(args, unsigned int *) = str_parse_uint(line->value[i++]); break;
		case 'f': *va_arg(args, double *) = str_parse_double(line->value[i++]); break;
		case 'b': *va_arg(args, bool *) = str_parse_bool(line->value[i++]); break;
		default: throw("Invalid format character '%c'.", format[-1]);
		}
	}

	va_end(args);

	cfg_line_delete(line);

	return true;
}

/**
 * Read a line from the reader.
 *   @reader: The reader.
 *   &returns: The line.
 */

_export
struct cfg_line_t *cfg_read_line(struct cfg_reader_t *reader)
{
	struct cfg_line_t *line;

	line = reader->buf;
	reader->buf = buf_line(reader);

	return line;
}

/**
 * Reader a two-part version from the reader.
 *   @reader: The reader.
 *   @key: The key to match.
 *   @maj: The major component.
 *   @min: The minor component.
 */

_export
void cfg_read_ver2(struct cfg_reader_t *reader, const char *key, unsigned int *maj, unsigned int *min)
{
	struct cfg_line_t *line;

	line = cfg_read_line(reader);
	if(line == NULL)
		throw("Missing version '%s'.", key);

	if(line->nvalues != 1)
		throw("Invalid version. Must have one argument.");

	str_parsef(line->value[0], "%u.%u%$", maj, min);

	cfg_line_delete(line);
}

/**
 * Read a single string from the reader. The returned string must be freed.
 *   @reader: The reader.
 *   @key: The key.
 *   &returns: The string or null.
 */

_export
char *cfg_read_str(struct cfg_reader_t *reader, const char *key)
{
	char *value;
	struct cfg_line_t *line;

	line = cfg_reader_get(reader, key);
	if(line == NULL)
		return NULL;

	if(line->nvalues != 1)
		throw("Too many values for '%s' directive.", key);

	value = line->value[0];
	line->nvalues = 0;
	cfg_line_delete(line);

	return value;
}


/**
 * Delete a line.
 *   @line: The line.
 */

_export
void cfg_line_delete(struct cfg_line_t *line)
{
	unsigned int i;

	for(i = 0; i < line->nvalues; i++)
		mem_free(line->value[i]);

	mem_free(line->key);
	mem_free(line);
}


/**
 * Read a line from the reader input.
 *   @reader: The reader.
 *   &returns: The line.
 */

static struct cfg_line_t *buf_line(struct cfg_reader_t *reader)
{
	char *str = NULL, *ptr, *value;
	struct cfg_line_t *line;

	do {
		char *ln = io_input_line(reader->input);
		str_set(&str, ln);
		if(str == NULL)
			return NULL;

		reader->line++;

		ptr = str_ltrimp(str);
	} while((*ptr == '#') || (*ptr == '\0'));

	line = mem_alloc(sizeof(struct cfg_line_t) + sizeof(char *));
	line->key = str_unquote(ptr, &ptr);
	line->value[0] = NULL;
	line->nvalues = 0;

	while((value = str_unquote(ptr, &ptr)) != NULL) {
		line = mem_realloc(line, sizeof(struct cfg_line_t) + (line->nvalues + 2) * sizeof(char *));
		line->value[line->nvalues++] = value;
		line->value[line->nvalues] = NULL;
	}

	mem_free(str);

	return line;
}
