#include "common.h"
#include "string.h"
#include <ctype.h>
#include <string.h>
#include "mem.h"


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
