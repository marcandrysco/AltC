#ifndef STRING_H
#define STRING_H

/*
 * character function declarations
 */


/*
 * string function declarations
 */

size_t str_len(const char *str);
char *str_chr(const char *str, char ch);
char *str_rchr(const char *str, char ch);

bool str_isequal(const char *left, const char *right);
int str_cmp(const char *left, const char *right);
bool str_chk(const char *left, const char *right);

char *str_prefix(const char *left, const char *right);
char *str_prefixi(const char *left, const char *right);

char *str_prequel(const char *left, const char *right, unsigned int min);
char *str_prequeli(const char *left, const char *right, unsigned int min);

bool str_isspace(char ch);
bool str_isdigit(char ch);
bool str_isalpha(char ch);
bool str_isalnum(char ch);

void str_tolower(char *str);
void str_toupper(char *str);

size_t str_ltrim(const char *str);
char *str_ltrimp(const char *str);

char *str_unquote(const char *str, char **endptr);

void str_copy(char *restrict dest, const char *restrict src);
char *str_dup(const char *str);
char *str_clone(const char *str);
void str_set(char **dest, char *src);
void str_replace(char **dest, const char *src);

void str_erase(char *str);

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

struct io_output_t str_output(char **str);

/*
 * parse function declarations
 */

int str_parse_int(const char *str);
unsigned int str_parse_uint(const char *str);
double str_parse_double(const char *str);
double str_parse_double_si(const char *str);
bool str_parse_bool(const char *str);
void str_parsef(const char *restrict str, const char *restrict format, ...);

unsigned int str_scan_uint(const char *str, char **endptr);
double str_scan_double(const char *str, char **endptr);

unsigned int str_read_uint(const char *str, char **endptr);
double str_read_double(const char *str, char **endptr);

/**
 * Retrieve the last character of a string.
 *   &returns: The last character or null on a zero length string.
 */

static inline char str_tail(const char *str)
{
	size_t len;

	len = str_len(str);
	return len ? str[len - 1] : '\0';
}


/**
 * Convert a character to lowercase.
 *   @ch: The character.
 *   &returns: The lowercase character.
 */

static inline char ch_tolower(char ch)
{
	return ((ch >= 'A') && (ch <= 'Z')) ? (ch - 'A' + 'a') : ch;
}

/**
 * Convert a character to uppercase.
 *   @ch: The character.
 *   &returns: The uppercase character.
 */

static inline char ch_toupper(char ch)
{
	return ((ch >= 'a') && (ch <= 'z')) ? (ch - 'a' + 'A') : ch;
}

#endif
