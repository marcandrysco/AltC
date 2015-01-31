#ifndef DYNLIB_H
#define DYNLIB_H

/*
 * dynamic library function declarations
 */

struct dynlib_t *dynlib_open(const char *name);
void dynlib_close(struct dynlib_t *lib);

void *dynlib_symbol(struct dynlib_t *lib, const char *sym);

#endif
