#ifndef TRY_H
#define TRY_H

/*
 * internal try function declarations
 */


jmp_buf *_tryjmp(void);
int _trycond(int val);
void nothrow();

/*
 * exception function declarations
 */

_noreturn void _throw(const char *restrict file, unsigned long line, const char *restrict format, ...);
_noreturn void _vthrow(const char *restrict file, unsigned long line, const char *restrict format, va_list args);

_noreturn void _fatal(const char *restrict file, unsigned long line, const char *restrict format, ...);
_noreturn void _vfatal(const char *restrict file, unsigned long line, const char *restrict format, va_list args);

/*
 * exception macros
 */

#define try() _trycond(!setjmp(*_tryjmp()))

#if _test || _debug
#	define throw(...) _throw(__FILE__, __LINE__, __VA_ARGS__)
#	define fatal(...) _fatal(__FILE__, __LINE__, __VA_ARGS__)
#	define vfatal(format, args) _vfatal(__FILE__, __LINE__, format, args)
#else
#	define throw(...) _throw(NULL, 1, __VA_ARGS__)
#	define fatal(...) _fatal(NULL, 1, __VA_ARGS__)
#	define vfatal(format, args) _vfatal(NULL, 1, format, args)
#endif

#endif
