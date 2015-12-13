#ifndef TYPES_FUNC_H
#define TYPES_FUNC_H

/*
 * common function declarations
 */

void *new_null();
void delete_noop(void *ref);

int compare_char(const void *left, const void *right);
int compare_int(const void *left, const void *right);
int compare_uint(const void *left, const void *right);
int compare_ptr(const void *left, const void *right);
int compare_str(const void *left, const void *right);

#endif
