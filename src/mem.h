#ifndef MEM_H
#define MEM_H

/*
 * memory function declarations
 */

void *mem_alloc(size_t size);
void *mem_realloc(void *ptr, size_t nbytes);
void _mem_set(void **dest, void *src);
void mem_free(void *ptr);
void _mem_freelc(unsigned int nptrs, void **ptrs);
void mem_erase(void *ptr);

void mem_copy(void *restrict dest, const void *restrict src, size_t nbytes);
void mem_move(void *dest, const void *src, size_t nbytes);
void mem_zero(void *dest, size_t nbytes);

/*
 * convenience macros
 */

#define mem_set(dest, src) _mem_set((void **)dest, src)
#define mem_freelc(nptrs, ptrs) _mem_freelc(nptrs, (void **)ptrs)

#define mem_getref(type, val) &(union { type v; }){ .v = (val) }.v
#define mem_dupval(type, val) mem_dup(&(union { type v; }){ .v = (val) }, sizeof(type))

#define getcontainer(ptr, type, member) ((type *)((void *)(ptr) - offsetof(type, member)))

#endif
