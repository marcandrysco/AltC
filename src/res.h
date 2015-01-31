#ifndef RES_H
#define RES_H

/*
 * resource function declarations
 */

struct res_info_t *res_info(void);
void res_check(void);
const char *res_error(void);

struct res_info_t *res_push(void);
void res_pop(void);

void res_clear(void);
void res_memclear(void);
void res_nodeclear(void);

void res_add(struct _res_node_t *node, ssize_t offset, void (*destroy)(void *));
void res_remove(struct _res_node_t *node);

/*
 * convenience macros
 */

#define errstr res_error()

#endif
