#include "../common.h"
#include "dynlib.h"
#include <dlfcn.h>
#include "../string.h"


/**
 * Open a dynamic library.
 *   @name: The library name or path.
 *   &returns: The library.
 */

_export
_dynlib_t _dlopen(const char *name)
{
	char lib[str_lprintf("lib%s.so", name) + 1];

	str_printf(lib, "lib%s.so", name);

	return dlopen(lib, RTLD_NOW);
}

/**
 * Close a dynamic library.
 *   @lib: The library.
 */

_export
void _dlclose(_dynlib_t lib)
{
	dlclose(lib);
}

/**
 * Retrieve a symbol from from the library.
 *   @lib: The library.
 *   @sym: The symbol name.
 *   &returns: The symbol or null.
 */

_export
void *_dlsym(_dynlib_t lib, const char *sym)
{
	return dlsym(lib, sym);
}
