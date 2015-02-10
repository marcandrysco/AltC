#include "common.h"
#include "try.h"
#include "io/output.h"
#include "io/print.h"
#include "res.h"
#include "string.h"


/**
 * Retrieve the jump information from the thread store.
 *   &returns: The jump buffer pointer.
 */

_export
jmp_buf *_tryjmp(void)
{
	struct res_info_t *info = res_info();

	info->fatal = false;

	return &info->jmpbuf;
}

/**
 * Process a try conditional.
 *   @val: The setjump value.
 *   &returns: The input value.
 */

_export
int _trycond(int val)
{
	return val;
}

/**
 * Create a no throw condition so future throws cause fatal errors.
 */

_export
void nothrow()
{
	res_info()->fatal = true;
}


/**
 * Throw an error, returning to the try branch.
 *   @file: The source file.
 *   @line: The target line.
 *   @format; The format.
 *   @...: The printf-style arguments.
 */

_export
_noreturn void _throw(const char *restrict file, unsigned long line, const char *restrict format, ...)
{
	struct res_info_t *info;
	va_list args;

	info = res_info();
	if((info == NULL) || info->fatal) {
		va_start(args, format);
		_vfatal(file, line, format, args);
	}

	if(info->error != NULL)
		free(info->error);

	va_start(args, format);
	info->error = malloc(str_vlprintf(format, args) + 1);
	va_end(args);

	va_start(args, format);
	str_vprintf(info->error, format, args);
	va_end(args);

	longjmp(info->jmpbuf, 1);
}

/**
 * Throw an error using a variable argument list, returning to the try branch.
 *   @file: The source file.
 *   @line: The target line.
 *   @format; The format.
 *   @args: The variable argument list.
 */

_export
_noreturn void _vthrow(const char *restrict file, unsigned long line, const char *restrict format, va_list args)
{
	struct res_info_t *info;
	va_list copy;

	info = res_info();
	if((info == NULL) || info->fatal)
		_vfatal(file, line, format, args);

	if(info->error != NULL)
		free(info->error);

	va_copy(copy, args);
	info->error = malloc(str_vlprintf(format, args) + 1);
	str_vprintf(info->error, format, copy);

	longjmp(info->jmpbuf, 1);
}


/**
 * Fatally abort a program.
 *   @file: The source file.
 *   @line: The target line.
 *   @format; The format.
 *   @...: The printf-style arguments.
 */

_export
_noreturn void _fatal(const char *restrict file, unsigned long line, const char *restrict format, ...)
{
	va_list args;

	va_start(args, format);
	_vfatal(file, line, format, args);
	va_end(args);
}

/**
 * Fatally abort a program using a variable argument list.
 *   @file: The source file.
 *   @line: The target line.
 *   @format; The format.
 *   @args: The variable argument list.
 */

_export
_noreturn void _vfatal(const char *restrict file, unsigned long line, const char *restrict format, va_list args)
{
	if(file != NULL)
		eprintf("%s:%u: ", file, line);

	evprintf(format, args);
	eprintf("\n");
	abort();
}
