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
			while((*str != '"') && (*str != '\0'))
				strbuf_store(&buf, *str++);

			if(*str == '\0')
				throw("Unterminated quote.");

			str++;
		}
		else {
			while(!str_isspace(*str) && (*str != '\0'))
				strbuf_store(&buf, *str++);
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
