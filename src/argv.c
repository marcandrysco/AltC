#include "common.h"
#include "types/strbuf.h"
#include "mem.h"
#include "string.h"
#include "try.h"


/**
 * Parse an argument array.
 *   @str: The input string.
 *   @argv: The output argument array.
 *   @argc: The output argument count.
 */

_export
void argv_parse(const char *str, char ***argv, unsigned int *argc)
{
	unsigned int n = 0;
	struct strbuf_t buf;

	*argv = mem_alloc(sizeof(char *));

	while(true) {
		while(str_isspace(*str))
			str++;

		if(*str == '\0')
			break;

		buf = strbuf_init(16);

		if(*str == '"') {
			str++;
			while((*str != '"') && (*str != '\0')) {
				if(*str == '\\')
					str++;

				strbuf_store(&buf, *str++);
			}

			if(*str == '\0')
				throw("Unterminated quote.");

			str++;
		}
		else {
			while(!str_isspace(*str) && (*str != '\0')) {
				if(*str == '\\')
					str++;

				strbuf_store(&buf, *str++);
			}
		}

		(*argv)[n++] = strbuf_done(&buf);
		*argv = mem_realloc(*argv, (n + 1) * sizeof(char *));
	}

	(*argv)[n] = NULL;

	if(argc != NULL)
		*argc = n;
}

/**
 * Delete an argument array.
 *   @argv: The argument array.
 */

_export
void argv_delete(char **argv)
{
	char **arg;

	for(arg = argv; *arg != NULL; arg++)
		mem_free(*arg);

	mem_free(argv);
}

/**
 * Serialize the arguments into a string.
 *   @argv: The argument array.
 *   &reutrns: The arguments in a single allocated string. Must be freed with
 *     'mem_free'.
 */

_export
char *argv_serial(char **argv)
{
	const char *str;
	struct strbuf_t buf;

	if(*argv == NULL)
		return NULL;

	buf = strbuf_init(32);

	while(true) {
		strbuf_store(&buf, '\"');

		for(str = *argv; *str != '\0'; str++) {
			if((*str == '"') || (*str == '\\'))
				strbuf_store(&buf, '\\');

			strbuf_store(&buf, *str);
		}

		strbuf_store(&buf, '\"');

		argv++;
		if(*argv == NULL)
			break;

		strbuf_store(&buf, ' ');
	}

	return strbuf_done(&buf);
}
