#include "common.h"
#include "string.h"
#include <ctype.h>
#include <string.h>
#include "io/parse.h"
#include "io/print.h"
#include "types/inc.h"
#include "mem.h"
#include "try.h"


struct output_t {
	char **str;
	struct strbuf_t buf;
};


/*
 * local function declarations
 */

static bool str_ctrl(void *ref, unsigned int id, void *data);
static void str_close(void *ref);
static size_t str_write(void *ref, const void *restrict buf, size_t nbytes);

static bool len_ctrl(void *ref, unsigned int id, void *data);
static void len_close(void *ref);
static size_t len_write(void *ref, const void *restrict buf, size_t nbytes);

static bool input_ctrl(void *ref, unsigned int id, void *data);
static void input_close(void *ref);
static size_t input_read(void *ref, void *restrict buf, size_t nbytes);

static bool output_ctrl(struct output_t *output, unsigned int id, void *data);
static void output_close(struct output_t *output);
static size_t output_write(struct output_t *output, void *restrict buf, size_t nbytes);


/**
 * Retrieve the string length.
 *   @str: The string.
 *   &returns: The length.
 */

_export
size_t str_len(const char *str)
{
	return strlen(str);
}

/**
 * Search for a character.
 *   @ch: The character.
 *   &returns: A pointer to the character if found, false otherwise.
 */

_export
char *str_chr(const char *str, char ch)
{
	return strchr(str, ch);
}

/**
 * Reverse search for a character.
 *   @ch: The character.
 *   &returns: A pointer to the character if found, false otherwise.
 */

_export
char *str_rchr(const char *str, char ch)
{
	return strrchr(str, ch);
}


/**
 * Check if two strings are equal.
 *   @left: The left string.
 *   @right: The right string.
 *   &returns: True if equal, false otherwise.
 */

_export
bool str_isequal(const char *left, const char *right)
{
	return strcmp(left, right) == 0;
}

/**
 * Compare two string.
 *   @left: The left string.
 *   @right: The right string.
 *   &returns: Their order.
 */

_export
int str_cmp(const char *left, const char *right)
{
	return strcmp(left, right);
}

/**
 * Check two string for equality, taking into account null values.
 *   @left: The left string.
 *   @right: The right string.
 *   &returns: True if equal, false otherwise.
 */

_export
bool str_chk(const char *left, const char *right)
{
	if(left == right)
		return true;
	else if((left == NULL) || (right == NULL))
		return false;
	return
		str_isequal(left, right);
}


/**
 * Check if a character is a space.
 *   @ch: The character.
 *   &returns: True if space, false otherwise.
 */

_export
bool str_isspace(char ch)
{
	return isspace(ch);
}

/**
 * Check if a character is a digit.
 *   @ch: The character.
 *   &returns: True if digit, false otherwise.
 */

_export
bool str_isdigit(char ch)
{
	return isdigit(ch);
}

/**
 * Check if a character is an alphabetical character.
 *   @ch: The character.
 *   &returns: True if alphabetical character, false otherwise.
 */

_export
bool str_isalpha(char ch)
{
	return isalpha(ch);
}

/**
 * Check if a character is an alphabetical character or number.
 *   @ch: The character.
 *   &returns: True if alphabetical character or number, false otherwise.
 */

_export
bool str_isalnum(char ch)
{
	return isalnum(ch);
}


/**
 * Trim whitespace from the left-hand side.
 *   @str: The string.
 *   &returns: The number of whitespace characters.
 */

_export
size_t str_ltrim(const char *str)
{
	size_t len = 0;

	while(isspace(*str))
		str++, len++;

	return len;
}

/**
 * Trim whitespace from the left-hand side, returning a pointer.
 *   @str: The string.
 *   &returns: A pointer to the first non-whitespace character.
 */

_export
char *str_ltrimp(const char *str)
{
	while(isspace(*str))
		str++;

	return (char *)str;
}


_export
char *str_unquote(const char *str, char **endptr)
{
	struct strbuf_t buf;

	str = str_ltrimp(str);
	if(*str == '\0')
		return NULL;

	buf = strbuf_init(str_len(str));

	if(*str == '"') {
		str++;

		while(*str != '"') {
			if(*str == '\0')
				throw("Unterminated quote.");

			strbuf_store(&buf, *str++);
		}

		str++;
	}
	else {
		while(!str_isspace(*str) && (*str != '\0'))
			strbuf_store(&buf, *str++);
	}

	*endptr = (char *)str;

	return strbuf_done(&buf);
}


/**
 * Copy a string to a destination.
 *   @dest: The destination.
 *   @src: The source.
 */

_export
void str_copy(char *restrict dest, const char *restrict src)
{
	strcpy(dest, src);
}

/**
 * Duplicate a string with a new allocated copy.
 *   @str: The original string.
 *   &returns: The duplicate copy.
 */

_export
char *str_dup(const char *str)
{
	char *dup;
	size_t len = str_len(str) + 1;

	dup = mem_alloc(len);
	str_copy(dup, str);

	return dup;
}

/**
 * Clone an input string, supporting nulls.
 *   @str: The string, may be null.
 *   &returns: A duplicate copy or null.
 */

_export
char *str_clone(const char *str)
{
	return str ? str_dup(str) : NULL;
}

/**
 * Set a string.
 *   @dest: The destination pointer.
 *   @src: Optional. Consumed. The source string.
 */

_export
void str_set(char **dest, char *src)
{
	str_erase(*dest);
	*dest = src;
}

/**
 * Replace the string in destination with source. Either string may be null.
 *   @dest: The destination.
 *   @src: The source.
 */

_export
void str_replace(char **dest, const char *src)
{
	mem_erase(*dest);
	*dest = str_clone(src);
}


/**
 * Erase a string if not null. 
 *   @str: The string.
 */

void str_erase(char *str)
{
	if(str != NULL)
		mem_free(str);
}


/**
 * Print a formatted string to a destination string.
 *   @str: The destination string.
 *   @format: The format.
 *   @...: The arguments.
 */

_export
void str_printf(char *restrict str, const char *restrict format, ...)
{
	va_list args;

	va_start(args, format);
	str_vprintf(str, format, args);
	va_end(args);
}

/**
 * Print a formatted string to a destination string.
 *   @str: The destination string.
 *   @format: The format.
 *   @args: The arguments.
 */

_export
void str_vprintf(char *restrict str, const char *restrict format, va_list args)
{
	static const struct io_output_i iface = { { str_ctrl, str_close }, str_write };

	io_vprintf((struct io_output_t){ (void *)&str, &iface }, format, args);
	*str = '\0';
}

/**
 * Handle a control signal for a string.
 *   @ref: The reference.
 *   @id: The control identifier.
 *   @data: The control data.
 *   &returns: True if the signal is handle, false otherwise.
 */

static bool str_ctrl(void *ref, unsigned int id, void *data)
{
	return false;
}

/**
 * Close the string.
 *   @ref: The reference.
 */

static void str_close(void *ref)
{
}

/**
 * Write to a string.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes written.
 */

static size_t str_write(void *ref, const void *restrict buf, size_t nbytes)
{
	char **str = ref;

	mem_copy(*str, buf, nbytes);
	*str += nbytes;

	return nbytes;
}


/**
 * Perform a formated print to determine the number of bytes written.
 *   @format: The format.
 *   @...: The arguments.
 *   &returns: The number of bytes, not including a terminate null.
 */

_export
size_t str_lprintf(const char *restrict format, ...)
{
	size_t nbytes;
	va_list args;

	va_start(args, format);
	nbytes = str_vlprintf(format, args);
	va_end(args);

	return nbytes;
}

/**
 * Perform a formated print to determine the number of bytes written.
 *   @format: The format.
 *   @args: The arguments.
 *   &returns: The number of bytes, not including a terminate null.
 */

_export
size_t str_vlprintf(const char *restrict format, va_list args)
{
	size_t len = 0;
	static const struct io_output_i iface = { { len_ctrl, len_close }, len_write };

	io_vprintf((struct io_output_t){ &len, &iface }, format, args);

	return len;
}

/**
 * Handle a control signal for a length.
 *   @ref: The reference.
 *   @id: The control identifier.
 *   @data: The control data.
 *   &returns: True if the signal is handle, false otherwise.
 */

static bool len_ctrl(void *ref, unsigned int id, void *data)
{
	return false;
}

/**
 * Close the length.
 *   @ref: The reference.
 */

static void len_close(void *ref)
{
}

/**
 * Write to a length.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes written.
 */

static size_t len_write(void *ref, const void *restrict buf, size_t nbytes)
{
	*(size_t *)ref += nbytes;

	return nbytes;
}


/**
 * Perform a formatted print into an allocated string.
 *   @format: The format.
 *   @...: The arguments.
 *   &returns: The allocated string.
 */

_export
char *str_aprintf(const char *restrict format, ...)
{
	char *str;
	va_list args;

	va_start(args, format);
	str = str_vaprintf(format, args);
	va_end(args);

	return str;
}

/**
 * Perform a formatted print into an allocated string.
 *   @format: The format.
 *   @args: The arguments.
 *   &returns: The allocated string.
 */

_export
char *str_vaprintf(const char *restrict format, va_list args)
{
	char *str;
	va_list copy;

	va_copy(copy, args);
	str = mem_alloc(str_vlprintf(format, copy) + 1);
	va_end(copy);

	str_vprintf(str, format, args);

	return str;
}


/**
 * Create an input from a string.
 *   @str: The string.
 *   &returns: The input.
 */

_export
struct io_input_t str_input(const char *restrict str)
{
	const char **ptr;
	static const struct io_input_i iface = { { input_ctrl, input_close }, input_read };

	ptr = mem_alloc(sizeof(char **));
	*ptr = str;

	return (struct io_input_t){ ptr, &iface };
}

/**
 * Create an input from a string.
 *   @ptr: The string pointer.
 *   &returns: The input.
 */

_export
struct io_input_t str_inputptr(const char *restrict *ptr)
{
	static const struct io_input_i iface = { { input_ctrl, delete_noop }, input_read };

	return (struct io_input_t){ (void *)ptr, &iface };
}

/**
 * Handle a control signal for a input.
 *   @ref: The reference.
 *   @id: The control identifier.
 *   @data: The control data.
 *   &returns: True if the signal is handle, false otherwise.
 */

static bool input_ctrl(void *ref, unsigned int id, void *data)
{
	return false;
}

/**
 * Close the input.
 *   @ref: The reference.
 */

static void input_close(void *ref)
{
	char **ptr = ref;

	mem_free(ptr);
}

/**
 * Read from a input.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes written.
 */

static size_t input_read(void *ref, void *restrict buf, size_t nbytes)
{
	size_t read = 0;
	const char **ptr = ref;

	while((**ptr != '\0') && (nbytes > 0)) {
		*(char *)(buf++) = *(*ptr)++;
		nbytes--;
		read++;
	}

	return read;
}


/**
 * Create an output to a string.
 *   @str: The string.
 *   &returns: The output.
 */

_export
struct io_output_t str_output(char **str)
{
	struct output_t *output;
	static const struct io_output_i iface = { { (io_ctrl_f)output_ctrl, (io_close_f)output_close }, (io_write_f)output_write };

	output = mem_alloc(sizeof(struct output_t));
	output->str = str;
	output->buf = strbuf_init(64);

	return (struct io_output_t){ output, &iface };
}

/**
 * Handle a control signal for a output.
 *   @ref: The reference.
 *   @id: The control identifier.
 *   @data: The control data.
 *   &returns: True if the signal is handle, false otherwise.
 */

static bool output_ctrl(struct output_t *output, unsigned int id, void *data)
{
	return false;
}

/**
 * Close the string output.
 *   @output: The output.
 */

static void output_close(struct output_t *output)
{
	*output->str = strbuf_done(&output->buf);
	mem_free(output);
}

/**
 * Write to an output.
 *   @ref: The reference.
 *   @buf: The buffer.
 *   @nbytes: The number of bytes to write.
 *   &returns: The number of bytes written.
 */

static size_t output_write(struct output_t *output, void *restrict buf, size_t nbytes)
{
	strbuf_write(&output->buf, buf, nbytes);

	return nbytes;
}


/**
 * Parse an unsigned integer from a string.
 *   @str: The string.
 *   &returns: The unsigned integer.
 */

_export
unsigned int str_parse_uint(const char *str)
{
	int16_t byte = -1;
	unsigned int val;

	val = io_parse_uint(str_inputptr(&str), &byte);
	if(byte >= 0)
		throw("Extra text after number.");

	return val;
}

/**
 * Parse a double from a string.
 *   @str: The string.
 *   &returns: The double.
 */

_export
double str_parse_double(const char *str)
{
	int16_t byte = -1;
	double val;

	val = io_parse_double(str_inputptr(&str), &byte);
	if(byte >= 0)
		throw("Extra text after number.");

	return val;
}


_export
void str_parsef(const char *restrict str, const char *restrict format, ...)
{
	va_list args;
	int16_t byte = -1;

	va_start(args, format);
	io_vparsef(str_inputptr(&str), &byte, format, args);
	va_end(args);
}


_export
unsigned int str_scan_uint(const char *str, char **endptr)
{
	unsigned int num;
	int16_t byte = -1;

	num = io_parse_uint(str_inputptr(&str), &byte);

	if(endptr != NULL)
		*endptr = (char *)((byte < 0) ? str : (str - 1));

	return num;
}
