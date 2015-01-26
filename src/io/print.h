#ifndef IO_PRINT_H
#define IO_PRINT_H

/**
 * Print callback modifiers structure.
 *   @zero, neg: Zero padding and negative flag.
 *   @width, frac: The field width and fractional size.
 */

struct io_print_mod_t {
	bool zero, neg;
	uint16_t width, frac;
};

/**
 * Printing callback funtion.
 *   @output: The output device.
 *   @mod: Modifiers.
 *   @args: The variable argument list. Read only the parameter expected.
 */

typedef void (*io_print_f)(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);

/**
 * Printing callback structure.
 *   @ch: The short character.
 *   @name: The long name. Set to 'NULL' if unused.
 *   @callback: The callback function.
 */

struct io_print_t {
	char ch;
	const char *name;
	io_print_f callback;
};


/*
 * printing variables
 */

extern struct io_print_t io_print_default[];

/*
 * printing function declarations
 */

void io_print_char(struct io_output_t output, char ch);
void io_print_str(struct io_output_t output, const char *str);

void io_printf(struct io_output_t output, const char *restrict format, ...);
void io_vprintf(struct io_output_t output, const char *restrict format, va_list args);
void io_vprintf_custom(struct io_output_t output, struct io_print_t *print, const char *format, struct arglist_t *args);
void io_printf_char(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_str(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_int(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_uint(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_hex(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_ptr(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_smartfp(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);
void io_printf_chunk(struct io_output_t output, struct io_print_mod_t *mod, struct arglist_t *list);

void io_format_str(struct io_output_t output, const char *str, uint16_t width, bool neg, char pad);
void io_format_int(struct io_output_t output, int value, uint8_t base, int16_t width, char pad);
void io_format_uint(struct io_output_t output, unsigned int value, uint8_t base, int16_t width, bool neg, char pad);
void io_format_smartfp(struct io_output_t output, double value, int16_t width, bool neg, char pad);
void io_format_chunk(struct io_output_t output, struct io_chunk_t chunk, uint16_t width, bool neg, char pad);

/*
 * convenience macros
 */

#define printf(...) io_printf(io_stdout, __VA_ARGS__)
#define vprintf(format, args) io_vprintf(io_stdout, format, args)
#define eprintf(...) io_printf(io_stderr, __VA_ARGS__)
#define evprintf(format, args) io_vprintf(io_stderr, format, args)

#endif
