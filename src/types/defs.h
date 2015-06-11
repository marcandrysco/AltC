#ifndef TYPES_DEFS_H
#define TYPES_DEFS_H

/**
 * Create callback.
 *   &returns: The pointer.
 */

typedef void *(*new_f)(void);

/**
 * Copy callback.
 *   @ptr: The original pointer.
 *   &returns: The copied pointer.
 */

typedef void *(*copy_f)(void *ptr);

/**
 * Delete callback.
 *   @ptr: The pointer.
 */

typedef void (*delete_f)(void *ptr);

/**
 * Compare two refrences.
 *   @left: The left reference.
 *   @right: The right reference.
 *   &returns: Their order.
 */

typedef int (*compare_f)(const void *left, const void *right);


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

/**
 * Generic AVL tree.
 *   @root: The root.
 *   @delete: Deletion function.
 */

struct avltree_t {
	struct avltree_root_t root;

	delete_f delete;
};

/**
 * Generic AVL tree instance.
 *   @ref: The stored reference.
 *   @delete: Delete function.
 *   @node: The tree node.
 */

struct avltree_inst_t {
	void *ref;
	delete_f delete;

	struct avltree_node_t node;
};


/**
 * String trie node structure.
 *   @ref: The stored reference.
 *   @str: The string piece.
 *   @tree: The subtree.
 *   @node: The node.
 */

struct strtrie_t {
	void *ref;
	char *str;
	struct avltree_root_t tree;
	struct avltree_node_t node;
};

#endif
