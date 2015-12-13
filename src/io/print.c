#include "../common.h"
#include "print.h"
#include "../dtoa.h"
#include "../math.h"
#include "../mem.h"
#include "../string.h"
#include "../try.h"
#include "chunk.h"
#include "output.h"


/*
 * global variables
 */

_export struct io_print_t io_print_default[] = {
	{ 'c', "char", io_printf_char },
	{ 's', "string", io_printf_str },
	{ 'd', "decimal", io_printf_int },
	{ 'i', "integer", io_printf_int },
	{ 'u', "unsigned", io_printf_uint },
	{ 'x', "hex", io_printf_hex },
	{ 'p', "pointer", io_printf_ptr },
	{ 'f', "float", io_printf_float },
	{ 'g', "smartfp", io_printf_smartfp },
	{ 'C', "chunk", io_printf_chunk },
	/*
	{ 'b', "binary", 0, io_printf_binary },
	{ 'u', "unsigned", 0, io_printf_uint },
	{ 'f', "float", 0, io_printf_float },
	*/
	{ '\0', NULL, NULL }
};


/**
 * Print a character to the output.
 *   @output: The output.
 *   @ch: The character.
 */

_export
void io_print_char(struct io_output_t output, char ch)
{
	io_output_full(output, &ch, sizeof(char));
}

/**
 * Print a string to the output.
 *   @output: The output.
 *   @str: The string.
 */

_export
void io_print_str(struct io_output_t output, const char *str)
{
	io_output_full(output, str, str_len(str));
}


/**
 * Print a formatted list.
 *   @output: The output the device.
 *   @format: The print-style format.
 *   @...: The print-style arguments.
 */

_export
void io_printf(struct io_output_t output, const char *restrict format, ...)
{
	va_list args;

	va_start(args, format);
	io_vprintf(output, format, args);
	va_end(args);
}

/**
 * Print a variable arugment formatted list.
 *   @output: The output the device.
 *   @format: The print-style format.
 *   @args: The print-style arguments.
 */

_export
void io_vprintf(struct io_output_t output, const char *restrict format, va_list args)
{
	struct arglist_t list;

	va_copy(list.args, args);
	io_vprintf_custom(output, io_print_default, format, &list);
	va_end(list.args);
}

/**
 * Print a formatted list using custom callacks.
 *   @output: The output the device.
 *   @print: The print callback table.
 *   @format: The print-style format.
 *   @...: The print-style arguments.
 */

_export
void io_vprintf_custom(struct io_output_t output, struct io_print_t *print, const char *format, struct arglist_t *args)
{
	size_t i = 0;
	struct io_print_t *search;
	struct io_print_mod_t mod;
	char name[16];

	while(*format != '\0') {
		if(*format == '%') {
			format++;

			if(*format != '%') {
				if(*format == '-')
					mod.neg = true, format++;
				else
					mod.neg = false;

				if(*format == '0')
					mod.zero = true, format++;
				else
					mod.zero = false;

				if(*format == '!') {
					mod.width = va_arg(args->args, unsigned int);
					format++;
				}
				else {
					mod.width = 0;
					while(str_isdigit(*format))
						mod.width = mod.width * 10 + *format - '0', format++;
				}

				mod.frac = 0;
				if(*format == '.') {
					format++;
					while(str_isdigit(*format))
						mod.frac = mod.frac * 10 + *format - '0', format++;
				}

				if(*format == ':') {
					format++;

					for(i = 0; format[i] != ':'; i++) {
						if(i == 15)
							throw("Format name too long.");
						else if(format[i] == '\0')
							throw("Invalid format, no ending ':'.");
					}

					mem_copy(name, format, i);
					name[i] = '\0';

					search = print;
					while(1) {
						if(search->callback == NULL)
							throw("Invalid token '%c'.", *format);
						if((search->name != NULL) && str_isequal(search->name, name))
							break;

						search++;
					}

					format += i;
				}
				else {
					search = print;
					while(search->ch != *format) {
						if(search->callback == NULL)
							throw("Invalid token '%c'.", *format);

						search++;
					}
				}

				search->callback(output, &mod, args);
			}
			else 
				io_print_char(output, '%');

			format++;
		}
		else {
			i = 0;
			do
				i++;
			while((format[i] != '%') && (format[i] != '\0'));

			io_output_write(output, format, i);
			format += i;
		}
	}
}

/**
 * Printf-style character output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_char(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	uint8_t ch;

	ch = va_arg(list->args, int);
	io_output_write(output, &ch, sizeof(uint8_t));
}

/**
 * Printf-style string output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_str(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_str(output, va_arg(list->args, const char *), mod->width, mod->neg, ' ');
}

/**
 * Printf-style integer output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_int(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_int(output, va_arg(list->args, int), 10, mod->width, mod->zero ? '0' : '\0');
}

/**
 * Printf-style unsigned integer output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_uint(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_uint(output, va_arg(list->args, unsigned int), 10, mod->width, mod->neg, mod->zero ? '0' : '\0');
}

/**
 * Printf-style unsigned integer output in hexidecimal.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_hex(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_uint(output, va_arg(list->args, unsigned int), 16, mod->width, mod->neg, mod->zero);
}

/**
 * Printf-style pointer output in hexidecimal.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_ptr(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_uint(output, (unsigned int)(intptr_t)va_arg(list->args, void *), 16, 2*sizeof(void *), mod->neg, '0');
}

/**
 * Printf-style floating-point output using a fixed decimal.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_float(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_float(output, va_arg(list->args, double), mod->width, mod->frac, mod->neg, ' ');
}

/**
 * Printf-style floating-point output using smart decimal.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_smartfp(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_smartfp(output, va_arg(list->args, double), mod->width, mod->neg, ' ');
}

/**
 * Printf-style chunk output.
 *   @device: The output device.
 *   @mod: The modifier.
 *   @args: The variable argument list with an upcoming string.
 */

_export
void io_printf_chunk(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list)
{
	io_format_chunk(output, va_arg(list->args, struct io_chunk_t), mod->width, mod->neg, ' ');
}


/**
 * Format a string.
 *   @output: The output device.
 *   @str: The string.
 *   @width: The width.
 *   @neg: Negative alignment.
 *   @pad: Padding character.
 */

_export
void io_format_str(struct io_output_t output, const char *str, uint16_t width, bool neg, char pad)
{
	size_t len;

	len = str_len(str);
	if(width > len) {
		uint16_t i;

		if(neg)
			io_output_write(output, str, len);

		for(i = len; i < width; i++)
			io_print_char(output, pad);

		if(!neg)
			io_output_write(output, str, len);
	}
	else if(width > 0)
		io_output_write(output, str, width);
	else
		io_output_write(output, str, len);
}

/**
 * Format a signed integer.
 *   @output: The output device.
 *   @value: The value.
 *   @base: The base.
 *   @width: The width.
 *   @pad: Padding character.
 */

_export
void io_format_int(struct io_output_t output, int value, uint8_t base, int16_t width, char pad)
{
	if(value < 0) {
		char neg = '-';

		if(width > 0)
			width--;

		value = -value;
		io_output_write(output, &neg, 1);
	}

	io_format_uint(output, value, base, width, false, pad);
}

/**
 * Format an unsigned integer.
 *   @output: The output device.
 *   @value: The value.
 *   @base: The base.
 *   @width: The width.
 *   @neg: Negative alignment.
 *   @pad: Padding character.
 */

_export
void io_format_uint(struct io_output_t output, unsigned int value, uint8_t base, int16_t width, bool neg, char pad)
{
	uint8_t i = 0;
	uint16_t len = m_max_uint16(width, 19);
	char buf[len];

	do {
		uint8_t val;

		val = value % base;
		buf[len - ++i] = val + ((val < 10) ? '0' : ('a' - 10));
	} while((value /= base) > 0);

	if(!neg) {
		while(i < width)
			buf[len - ++i] = pad ?: ' ';

		io_output_write(output, buf + len - i, width ?: i);
	}
	else {
		io_output_write(output, buf + len - i, (i <= width) ? i : width);

		if(width > i) {
			uint8_t rem = width - i;

			while(i < width)
				buf[len - ++i] = ' ';

			io_output_write(output, buf + len - i, rem);
		}
	}
}

/**
 * Format a floating-point number.
 *   @output: The output device.
 *   @value: The value.
 *   @base: The base.
 *   @width: The width.
 *   @frac: The fractional size.
 *   @neg: Negative alignment.
 *   @pad: Padding character.
 */

_export
void io_format_float(struct io_output_t output, double value, int16_t width, uint16_t frac, bool neg, char pad)
{
	char buf[400], *str;

	if(isnan(value))
		str_copy(buf, "NaN");
	else if(value == INFINITY)
		str_copy(buf, "Inf");
	else if(value == -INFINITY)
		str_copy(buf, "-Inf");
	else {
		bool opt;
		char tmp[35];
		int16_t i, exp, end;

		str = buf;

		if(value < 0)
			*str++ = '-', value = -value;

		if(value > 0) {
			exp = errol1_dtoa(value, tmp, &opt) - 1;
			end = exp - str_len(tmp);
		}
		else
			exp = INT16_MIN, end = INT16_MIN;

		for(i = ((exp > 0) ? exp : 0); i >= -frac; i--) {
			*str++ = ((i <= exp) && (i > end)) ? tmp[exp-i] : '0';

			if(i == 0)
				*str++ = '.';
		}

		*str = '\0';
	}

	if(str_len(buf) < width) {
		uint16_t i;

		for(i = str_len(buf); i < width; i++)
			io_print_char(output, pad);
	}

	io_print_str(output, buf);
}

/**
 * Format a floating-point number.
 *   @output: The output device.
 *   @value: The value.
 *   @base: The base.
 *   @width: The width.
 *   @neg: Negative alignment.
 *   @pad: Padding character.
 */

_export
void io_format_smartfp(struct io_output_t output, double value, int16_t width, bool neg, char pad)
{
	int16_t exp;
	char buf[200];

	if(isnan(value))
		io_printf(output, "NaN");
	else if(value == INFINITY)
		io_printf(output, "Inf");
	else if(value == -INFINITY)
		io_printf(output, "-Inf");
	else if(value != 0.0) {
		bool opt;

		if(value < 0)
			io_print_char(output, '-'), value = -value;

		exp = errol1_dtoa(value, buf, &opt);

		if(exp == 1)
			io_printf(output, "%c.%c%s", buf[0], buf[1] ?: '0', buf[1] ? buf+2 : "");
		else
			io_printf(output, "%c.%c%se%d", buf[0], buf[1] ?: '0', buf[1] ? buf+2 : "", exp-1);
	}
	else
		io_printf(output, "0.0");
}

/**
 * Format a chunk.
 *   @output: The output device.
 *   @chunk: The chunk.
 *   @width: The width.
 *   @neg: Negative alignment.
 *   @pad: Padding character.
 */

_export
void io_format_chunk(struct io_output_t output, struct io_chunk_t chunk, uint16_t width, bool neg, char pad)
{
	if(width > 0) {
		size_t len = io_chunk_proc_len(chunk);
		if(width > len) {
			uint16_t i;

			if(neg)
				io_chunk_proc(chunk, output);

			for(i = len; i < width; i++)
				io_output_char(output, pad);

			if(!neg)
				io_chunk_proc(chunk, output);
		}
		else
			io_chunk_proc(chunk, output);
	}
	else
		io_chunk_proc(chunk, output);
}
