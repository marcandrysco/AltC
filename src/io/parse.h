#ifndef IO_PARSE_H
#define IO_PARSE_H

/*
 * parsing function declarations
 */

unsigned int io_parse_trim(struct io_input_t input, int16_t *byte);
unsigned int io_parse_uint(struct io_input_t input, int16_t *byte);
double io_parse_double(struct io_input_t input, int16_t *byte);

void io_parsef(struct io_input_t input, int16_t *byte, const char *restrict format, ...);
void io_vparsef(struct io_input_t input, int16_t *byte, const char *restrict format, va_list args);

/*
 * scanning function declarations
 */

unsigned int io_scanf(struct io_input_t input, int16_t *byte, const char *restrict format, ...);
unsigned int io_vscanf(struct io_input_t input, int16_t *byte, const char *restrict format, va_list args);

#endif
