#ifndef STRING_H
#define STRING_H

/*
 * string function declarations
 */

size_t str_len(const char *str);
char *str_rchr(const char *str, char ch);

bool str_isequal(const char *left, const char *right);
int str_cmp(const char *left, const char *right);

bool str_isdigit(char ch);
bool str_isspace(char ch);

size_t str_ltrim(const char *str);
char *str_ltrimp(const char *str);

char *str_unquote(const char *str, char **endptr);

void str_copy(char *restrict dest, const char *restrict src);
char *str_dup(const char *str);

void str_erase(char *str);
void str_set(char **dest, char *src);

/*
 * i/o function declarations
 */

void str_printf(char *restrict str, const char *restrict format, ...);
void str_vprintf(char *restrict str, const char *restrict format, va_list args);

size_t str_lprintf(const char *restrict format, ...);
size_t str_vlprintf(const char *restrict format, va_list args);

char *str_aprintf(const char *restrict format, ...);
char *str_vaprintf(const char *restrict format, va_list args);

struct io_input_t str_input(const char *restrict str);
struct io_input_t str_inputptr(const char *restrict *ptr);

/*
 * parse function declarations
 */

unsigned int str_parse_uint(const char *str);
void str_parsef(const char *restrict str, const char *restrict format, ...);

unsigned int str_scan_uint(const char *str, char **endptr);

#endif
