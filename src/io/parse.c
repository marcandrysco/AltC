#include "../common.h"
#include "parse.h"
#include "../string.h"
#include "../try.h"
#include "input.h"


/*
 * local function declarations
 */

static int8_t convdigit(int16_t byte);


/**
 * Parse whitespace.
 *   @input: The input.
 *   @byte: The buffered byte.
 *   &returns: The amount of whitespace parsed.
 */

_export
unsigned int io_parse_trim(struct io_input_t input, int16_t *byte)
{
	unsigned int cnt;

	if(*byte < 0)
		*byte = io_input_byte(input);

	for(cnt = 0; str_isspace(*byte); cnt++)
		*byte = io_input_byte(input);

	return cnt;
}

/**
 * Parse an unsigned integer from the input.
 *   @input: The input.
 *   @byte: The buffered byte.
 *   &returns: The unsigned integer.
 */

_export
unsigned int io_parse_uint(struct io_input_t input, int16_t *byte)
{
	int8_t val, base = 10;
	unsigned long num = 0;

	if(*byte < 0)
		*byte = io_input_byte(input);

	if((*byte == '\0') || (*byte < 0))
		throw("Unexpected end of input.");

	if(*byte == '0') {
		*byte = io_input_byte(input);
		if(*byte == 'x') {
			base = 16;
			*byte = io_input_byte(input);
		}
		else if(*byte == 'b'){ 
			base = 2;
			*byte = io_input_byte(input);
		}
		else if(str_isdigit(*byte))
			base = 8;
		else
			return 0;
	}
	else if(!str_isdigit(*byte))
		throw("Invalid number.");

	while((val = convdigit(*byte)) != -1) {
		if(val >= base)
			break;

		num = base * num + val;
		*byte = io_input_byte(input);
	}

	return num;
}

/**
 * Parse a double from the input.
 *   @input: The input.
 *   @byte: The buffered byte.
 *   &returns: The double.
 */

_export
double io_parse_double(struct io_input_t input, int16_t *byte)
{
	char buf[256], *endptr;
	uint8_t i = 0;
	double val;

	if(*byte < 0)
		*byte = io_input_byte(input);

	if(*byte == '-')
		buf[i++] = *byte, *byte = io_input_byte(input);

	while(str_isdigit(*byte))
		buf[i++] = *byte, *byte = io_input_byte(input);

	if(*byte == '.') {
		buf[i++] = *byte, *byte = io_input_byte(input);

		while(str_isdigit(*byte))
			buf[i++] = *byte, *byte = io_input_byte(input);

		if(*byte == 'e') {
			buf[i++] = *byte, *byte = io_input_byte(input);

			if(*byte == '-')
				buf[i++] = *byte, *byte = io_input_byte(input);

			while(str_isdigit(*byte))
				buf[i++] = *byte, *byte = io_input_byte(input);
		}
	}

	if(i == 0)
		throw("Invalid number.");

	buf[i] = '\0';
	errno = 0;
	val = strtod(buf, &endptr);
	if(errno != 0)
		throw("Invalid number.");

	return val;
}

/**
 * Parse a boolean from the string 'true' or 'false'.
 *   @input: The input.
 *   @byte: The buffered byte.
 *   &returns: The boolean.
 */

_export
bool io_parse_bool(struct io_input_t input, int16_t *byte)
{
	unsigned int i;
	bool val;
	const char *lower = "rue", *upper = "RUE";
	if(*byte < 0)
		*byte = io_input_byte(input);

	if((*byte == 't') || (*byte == 'T')) {
		val = true;
		lower = "rue";
		upper = "RUE";
	}
	else if((*byte == 'f') || (*byte == 'F')) {
		val = false;
		lower = "alse";
		upper = "ALSE";
	}
	else
		throw("Invalid boolean value.");

	for(i = 0; lower[i] != '\0'; i++) {
		*byte = io_input_byte(input);
		if((*byte != lower[i]) && (*byte != upper[i]))
			throw("Invalid boolean value. %c vs %c", *byte, lower[i]);
	}

	*byte = io_input_byte(input);

	return val;
}


_export
unsigned int io_scanf(struct io_input_t input, int16_t *byte, const char *restrict format, ...)
{
	va_list args;
	unsigned int cnt;

	va_start(args, format);
	cnt = io_vscanf(input, byte, format, args);
	va_end(args);

	return cnt;
}


_export
void io_parsef(struct io_input_t input, int16_t *byte, const char *restrict format, ...)
{
	va_list args;

	va_start(args, format);
	io_vparsef(input, byte, format, args);
	va_end(args);
}

_export
void io_vparsef(struct io_input_t input, int16_t *byte, const char *restrict format, va_list args)
{
	const char *ptr;
	unsigned int cnt, target = 0;

	ptr = format;

	while(*ptr != '\0') {
		if(*ptr == '%') {
			ptr++;
			if(*ptr != '%')
				target++;
		}
		ptr++;
	}

	cnt = io_vscanf(input, byte, format, args);
	if(cnt != target)
		throw("Failed to parse input.");
}

_export
unsigned int io_vscanf(struct io_input_t input, int16_t *byte, const char *restrict format, va_list args)
{
	unsigned int cnt = 0;

	if(*byte < 0)
		*byte = io_input_byte(input);

	while(format != NULL) {
		if(*format == '%') {
			format++;
			if(*format == 'u') {
				*va_arg(args, unsigned int *) = io_parse_uint(input, byte);
			}
			else if(*format == '$') {
				if(*byte >= 0)
					return cnt;
			}
			else if(*format == '%') {
				if(*byte == *format)
					*byte = io_input_byte(input);
				else
					return cnt;
			}
			else
				throw("Invalid format.");

			cnt++;
			format++;
		}
		else if(*byte == *format)
			format++, *byte = io_input_byte(input);
		else
			return cnt;
	}

	return cnt;
}


/**
 * Convert a digit from a byte.
 *   @byte: The byte.
 *   &returns: The digit or a negative value.
 */

static int8_t convdigit(int16_t byte)
{
	if((byte >= '0') && (byte <= '9'))
		return byte - '0';
	else if((byte >= 'a') && (byte <= 'z'))
		return byte - 'a' + 10;
	else if((byte >= 'A') && (byte <= 'Z'))
		return byte - 'a' + 10;
	return
		-1;
}
