#include "common.h"
#include "try.h"
#include "io/output.h"
#include "io/print.h"
#include "res.h"


/**
 * Retrieve the jump information from the thread store.
 *   &returns: The jump buffer pointer.
 */

_export
jmp_buf *_tryjmp(void)
{
	return &res_info()->jmpbuf;
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

	va_start(args, format);

	info = res_info();
	if((info == NULL) || info->fatal)
		_vfatal(file, line, format, args);
	else
		;

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

	info = res_info();
	if((info == NULL) || info->fatal)
		_vfatal(file, line, format, args);
	else
		;

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
