#ifndef RES_H
#define RES_H

/*
 * resource function declarations
 */

struct res_info_t *res_info(void);
void res_check(void);

struct res_info_t *res_push(void);
void res_pop(void);

void res_clear(void);
void res_memclear(void);
void res_nodeclear(void);

#endif
