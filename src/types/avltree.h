#ifndef AVLTREE_H
#define AVLTREE_H

/*
 * avl tree definitions.
 */

#define AVLTREE_MAX_HEIGHT	48


/*
 * avl tree root function declarations
 */

struct avltree_root_t avltree_root_init(compare_f compare);
void avltree_root_destroy(struct avltree_root_t *root, ssize_t offset, delete_f delete);

struct avltree_node_t *avltree_root_first(struct avltree_root_t *root);
struct avltree_node_t *avltree_root_last(struct avltree_root_t *root);

struct avltree_node_t *avltree_root_lookup(struct avltree_root_t *root, const void *ref);
struct avltree_node_t *avltree_root_atleast(struct avltree_root_t *root, const void *ref);
struct avltree_node_t *avltree_root_atmost(struct avltree_root_t *root, const void *ref);

void avltree_root_insert(struct avltree_root_t *root, struct avltree_node_t *node);
struct avltree_node_t *avltree_root_remove(struct avltree_root_t *root, const void *ref);

/*
 * avl tree node function declaratoins
 */

struct avltree_node_t avltree_node_init(void *ref);

struct avltree_node_t *avltree_node_prev(struct avltree_node_t *node);
struct avltree_node_t *avltree_node_next(struct avltree_node_t *node);

/*
 * avl tree function declarations
 */

struct avltree_t avltree_init(compare_f compare, delete_f delete);
void avltree_destroy(struct avltree_t *tree);

void *avltree_lookup(struct avltree_t *tree, const void *key);
void avltree_insert(struct avltree_t *tree, const void *key, void *ref);

struct avltree_inst_t *avltree_first(struct avltree_t *tree);
struct avltree_inst_t *avltree_last(struct avltree_t *tree);
struct avltree_inst_t *avltree_prev(struct avltree_inst_t *inst);
struct avltree_inst_t *avltree_next(struct avltree_inst_t *inst);

struct avltree_t avltree_strlist(const char *const *list);

#endif
