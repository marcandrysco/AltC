#ifndef POSIX_DYNLIB_H
#define POSIX_DYNLIB_H

/*
 * dynamic library function declaratoins
 */

_dynlib_t _dlopen(const char *name);
void _dlclose(_dynlib_t lib);
void *_dlsym(_dynlib_t lib, const char *sym);

#endif
