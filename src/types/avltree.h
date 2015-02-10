#ifndef AVLTREE_H
#define AVLTREE_H

/*
 * avl tree definitions.
 */

#define AVLTREE_MAX_HEIGHT	48


/**
 * AVL tree root structure.
 *   @count: The number of nodes.
 *   @node: The root node.
 *   @compare: The comparison callback.
 */

struct avltree_root_t {
	unsigned int count;
	struct avltree_node_t *node;

	compare_f compare;
};

/**
 * AVL tree node storage.
 *   @balance: The current balance of the node, between '-2' to '2'.
 *   @ref: The reference.
 *   @parent, child: The parent and child nodes.
 */

struct avltree_node_t {
	int8_t balance;

	void *ref;
	struct avltree_node_t *parent, *child[2];
};


/*
 * avl tree root function declarations
 */

struct avltree_root_t avltree_root_init(compare_f compare);
void avltree_root_destroy(struct avltree_root_t *root, ssize_t offset, delete_f delete);

struct avltree_node_t *avltree_root_first(struct avltree_root_t *root);
struct avltree_node_t *avltree_root_last(struct avltree_root_t *root);

struct avltree_node_t *avltree_root_lookup(struct avltree_root_t *root, const void *ref);

void avltree_root_insert(struct avltree_root_t *root, struct avltree_node_t *node);
struct avltree_node_t *avltree_root_remove(struct avltree_root_t *root, const void *ref);

/*
 * avl tree node function declaratoins
 */

struct avltree_node_t avltree_node_init(void *ref);

struct avltree_node_t *avltree_node_prev(struct avltree_node_t *node);
struct avltree_node_t *avltree_node_next(struct avltree_node_t *node);

#endif
