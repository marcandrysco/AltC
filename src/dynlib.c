#include "common.h"
#include "dynlib.h"
#include "posix/dynlib.h"
#include "mem.h"
#include "res.h"
#include "try.h"


/**
 * Dynamic library structure.
 *   @ref: The library reference.
 *   @node: The resource node.
 */

struct dynlib_t {
	_dynlib_t ref;
	struct _res_node_t node;
};


/**
 * Open a dynamic library.
 *   @name: The library name.
 *   &returns: The library.
 */

_export
struct dynlib_t *dynlib_open(const char *name)
{
	_dynlib_t ref;
	struct dynlib_t *lib;

	ref = _dlopen(name);
	if(ref == NULL)
		throw("Cannot open library '%s'.", name);

	lib = mem_alloc(sizeof(struct dynlib_t));
	lib->ref = ref;
	res_add(&lib->node, offsetof(struct dynlib_t, node), (delete_f)dynlib_close);

	return lib;
}

/**
 * Close a dynamic library.
 *   @lib: The library.
 */

_export
void dynlib_close(struct dynlib_t *lib)
{
	res_remove(&lib->node);
	_dlclose(lib->ref);
	mem_free(lib);
}


/**
 * Retrieve a symbol the from the dynamic library.
 *   @lib: The library.
 *   @sym: The symbol name.
 *   &returns: The symbol or null.
 */

_export
void *dynlib_symbol(struct dynlib_t *lib, const char *sym)
{
	return _dlsym(lib->ref, sym);
}
