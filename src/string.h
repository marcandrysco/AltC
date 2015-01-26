#ifndef STRING_H
#define STRING_H

/*
 * string function declarations
 */

size_t str_len(const char *str);

bool str_isequal(const char *left, const char *right);
int str_cmp(const char *left, const char *right);

bool str_isdigit(char ch);

void str_copy(char *restrict dest, const char *restrict src);
char *str_dup(const char *str);

#endif
