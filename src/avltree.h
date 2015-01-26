#ifndef AVLTREE_H
#define AVLTREE_H

/*
 * avl tree definitions.
 */

#define AVLTREE_MAX_HEIGHT	48
#define AVLTREE_NODE_INIT	(struct avltree_node_t){ 0, NULL, { NULL, NULL } }


/**
 * AVL tree root structure.
 *   @node: The root node.
 */

struct avltree_root_t {
	struct avltree_node_t *node;
};

/**
 * AVL tree node storage.
 *   @balance: The current balance of the node, between '-2' to '2'.
 *   @parent, child: The parent and child nodes.
 */

struct avltree_node_t {
	int8_t balance;
	struct avltree_node_t *parent, *child[2];
};


/**
 * Comparison callback between a node and a key.
 *   @key: The input key.
 *   @node: The compared-to node.
 *   @arg: The callback argument.
 *   &returns: An integer representing their order.
 */

typedef int (*avltree_nodekey_f)(const void *key, const struct avltree_node_t *node, void *arg);

/**
 * Comparison callback between two nodes.
 *   @n1: The first node.
 *   @n2: The seconde node.
 *   @arg: The callback argument.
 *   &returns: An integer representing their order.
 */

typedef int (*avltree_nodenode_f)(const struct avltree_node_t *n1, const struct avltree_node_t *n2, void *arg);


/*
 * avl tree root function declarations
 */

struct avltree_root_t avltree_root_init();

struct avltree_node_t *avltree_root_first(struct avltree_root_t *root);
struct avltree_node_t *avltree_root_last(struct avltree_root_t *root);

struct avltree_node_t *avltree_root_lookup(struct avltree_root_t *root, const void *key, avltree_nodekey_f compare, void *arg);

void avltree_root_insert(struct avltree_root_t *root, struct avltree_node_t *node, avltree_nodenode_f compare, void *arg);
struct avltree_node_t *avltree_root_remove(struct avltree_root_t *root, const void *key, avltree_nodekey_f compare, void *arg);

#endif
