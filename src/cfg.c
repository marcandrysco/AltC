#include "common.h"
#include "cfg.h"
#include "mem.h"
#include "string.h"
#include "try.h"
#include "io/inc.h"


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
 *   @write: The writer.
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
	return (struct cfg_reader_t){ input, 0, 0l };
}

/**
 * Destroy a configuration reader.
 *   @reader: The reader.
 */

_export
void cfg_reader_destroy(struct cfg_reader_t *reader)
{
}


/**
 * Read a line from the reader.
 *   @reader: The reader.
 */

_export
struct cfg_line_t *cfg_read_line(struct cfg_reader_t *reader)
{
	char *str = NULL, *ptr, *value;
	struct cfg_line_t *line;

	do {
		str_set(&str, io_input_line(reader->input));
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
//_export
//void cfg_read_ver2(


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
