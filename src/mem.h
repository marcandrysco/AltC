#ifndef MEM_H
#define MEM_H

/*
 * memory function declarations
 */

void *mem_alloc(size_t size);
void mem_free(void *ptr);
void mem_erase(void *ptr);

void mem_copy(char *restrict dest, const char *restrict src, size_t nbytes);
void mem_move(char *dest, const char *src, size_t nbytes);

/*
 * convenience macros
 */

#define mem_getref(type, val) (&(union { type v; }){ .v = (val) })
#define mem_dupval(type, val) mem_dup(&(union { type v; }){ .v = (val) }, sizeof(type))

#define getcontainer(ptr, type, member) ((type *)((void *)(ptr) - offsetof(type, member)))

#endif
