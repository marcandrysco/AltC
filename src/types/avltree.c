#include "../common.h"
#include "avltree.h"
#include "../mem.h"
#include "../try.h"
#include "func.h"


/*
 * node definitions and macros.
 */

#define LEFT	0
#define RIGHT	1
#define CMP2NODE(cmp)	(cmp > 0 ? RIGHT : LEFT)
#define OTHERNODE(node)	(1 - node)
#define NODEDIR(node)	(node == RIGHT ? 1 : -1)

/*
 * local function declarations
 */

static void *getref(struct avltree_node_t *node, ssize_t offset);

static struct avltree_node_t *rotate_single(struct avltree_node_t *node, uint8_t dir);
static struct avltree_node_t *rotate_double(struct avltree_node_t *node, uint8_t dir);

static struct avltree_inst_t *inst_new(const void *key, void *ref, delete_f delete);
static void inst_delete(struct avltree_inst_t *inst);


/**
 * Create an empty AVL tree root.
 *   @compare: The comparison.
 *   &returns: The empty root.
 */

_export
struct avltree_root_t avltree_root_init(compare_f compare)
{
	return (struct avltree_root_t){ 0, NULL, compare };
}

/**
 * Destroy an AVL tree root.
 *   @root: The root.
 *   @offset; The offset.
 *   @delete: Deletion callack.
 */

_export
void avltree_root_destroy(struct avltree_root_t *root, ssize_t offset, delete_f delete)
{
	short i;
	struct avltree_node_t *stack[AVLTREE_MAX_HEIGHT];
	uint8_t stack_s[AVLTREE_MAX_HEIGHT];

	if(root->node == NULL)
		return;

	stack[0] = root->node;
	stack_s[0] = 0;

	for(i = 0; i >= 0; ) {
		switch(stack_s[i]) {
		case 0:
			stack_s[i]++;
			if(stack[i]->child[LEFT] != NULL) {
				stack[i+1] = stack[i]->child[LEFT];
				stack_s[i+1] = 0;
				i++;

				break;
			}

		case 1:
			stack_s[i]++;
			if(stack[i]->child[RIGHT] != NULL) {
				stack[i+1] = stack[i]->child[RIGHT];
				stack_s[i+1] = 0;
				i++;

				break;
			}

		case 2:
			delete(getref(stack[i--], offset));
		}
	}
}


/**
 * Obtain the first element from the root.
 *   @root: The root.
 *   &returns: The first node from the root or null.
 */

_export
struct avltree_node_t *avltree_root_first(struct avltree_root_t *root)
{
	struct avltree_node_t *node = root->node;

	if(node == NULL)
		return NULL;

	while(node->child[LEFT] != NULL)
		node = node->child[LEFT];

	return node;
}

/**
 * Obtain the last element from the root.
 *   @root: The root.
 *   &returns: The last node from the root or null.
 */

_export
struct avltree_node_t *avltree_root_last(struct avltree_root_t *root)
{
	struct avltree_node_t *node = root->node;

	if(node == NULL)
		return NULL;

	while(node->child[RIGHT] != NULL)
		node = node->child[RIGHT];

	return node;
}


/**
 * Look up an AVL tree node from the root.
 *   @root: The root.
 *   @key: The sought reference.
 *   &returns: The node if found, null otherwise.
 */

_export
struct avltree_node_t *avltree_root_lookup(const struct avltree_root_t *root, const void *ref)
{
	int cmp;
	struct avltree_node_t *node = root->node;

	while(node != NULL) {
		cmp = root->compare(ref, node->ref);
		if(cmp == 0)
			return node;
		else
			node = node->child[CMP2NODE(cmp)];
	}

	return NULL;
}


/**
 * Insert an AVL tree node to the root.
 *   @root: The tree root.
 *   @node: The node to insert.
 *   @arg: An argument passed to the comparison function.
 */

_export
void avltree_root_insert(struct avltree_root_t *root, struct avltree_node_t *node)
{
	int cmp;
	short i;
	uint8_t dir[AVLTREE_MAX_HEIGHT];
	struct avltree_node_t *stack[AVLTREE_MAX_HEIGHT];

	if(root->node == NULL) {
		root->count++;
		root->node = node;
		node->parent = NULL;

		return;
	}

	stack[0] = root->node;

	for(i = 0; stack[i] != NULL && i < AVLTREE_MAX_HEIGHT; i++) {
		cmp = root->compare(node->ref, stack[i]->ref);
		if(cmp == 0)
			throw("Node already exists.");

		dir[i] = CMP2NODE(cmp);
		stack[i+1] = stack[i]->child[dir[i]];
	}

	i--;
	stack[i]->child[dir[i]] = node;
	stack[i]->balance += NODEDIR(dir[i]);
	node->parent = stack[i];
	root->count++;

	if(stack[i]->child[OTHERNODE(dir[i])] != NULL)
		return;

	while(i-- > 0) {
		struct avltree_node_t *node;

		stack[i]->balance += NODEDIR(dir[i]);

		if(stack[i]->balance == 0)
			break;

		if((stack[i]->balance > -2) && (stack[i]->balance < 2))
			continue;

		if(dir[i+1] == CMP2NODE(stack[i]->balance))
			node = rotate_single(stack[i], OTHERNODE(CMP2NODE(stack[i]->balance)));
		else
			node = rotate_double(stack[i], OTHERNODE(CMP2NODE(stack[i]->balance)));

		if(i == 0)
			root->node = node;
		else
			stack[i-1]->child[dir[i-1]] = node;
		
		break;
	}
}

/**
 * Remove an AVL tree node from the tree root. If not found, no node is removed.
 *   @root: The tree root.
 *   @ref: The sought reference.
 *   &returns: The node if found, null otherwise.
 */

_export
struct avltree_node_t *avltree_root_remove(struct avltree_root_t *root, const void *ref)
{
	int cmp;
	short i, ii;
	uint8_t dir[AVLTREE_MAX_HEIGHT];
	struct avltree_node_t *stack[AVLTREE_MAX_HEIGHT], *node, *retval;

	if(root->node == NULL)
		return NULL;

	stack[0] = root->node;

	for(i = 0; stack[i] != NULL && i < AVLTREE_MAX_HEIGHT; i++) {
		cmp = root->compare(ref, stack[i]->ref);
		if(cmp == 0)
			break;

		dir[i] = CMP2NODE(cmp);
		stack[i+1] = stack[i]->child[dir[i]];
	}
	if(stack[i] == NULL)
		return NULL;

	dir[i] = CMP2NODE(stack[i]->balance);

	ii = i;
	node = stack[i]->child[dir[i]];
	if(node != NULL) {
		while(node->child[OTHERNODE(dir[ii])] != NULL) {
			i++;
			stack[i] = node;
			dir[i] = OTHERNODE(dir[ii]);
			node = node->child[dir[i]];
		}

		stack[i]->child[dir[i]] = node->child[dir[ii]];
		if(node->child[dir[ii]] != NULL)
			node->child[dir[ii]]->parent = stack[i];

		i++;

		node->child[LEFT] = stack[ii]->child[LEFT];
		node->child[RIGHT] = stack[ii]->child[RIGHT];
		node->balance = stack[ii]->balance;

		if(node->child[LEFT] != NULL)
			node->child[LEFT]->parent = node;

		if(node->child[RIGHT] != NULL)
			node->child[RIGHT]->parent = node;
	}

	if(ii == 0) {
		root->node = node;

		if(node != NULL)
			node->parent = NULL;
	}
	else {
		stack[ii-1]->child[dir[ii-1]] = node;

		if(node != NULL)
			node->parent = stack[ii-1];
	}

	retval = stack[ii];
	stack[ii] = node;

	while(i-- > 0) {
		stack[i]->balance -= NODEDIR(dir[i]);

		if((stack[i]->balance > 1) || (stack[i]->balance < -1)) {
			if(stack[i]->balance == -2 * stack[i]->child[CMP2NODE(stack[i]->balance/2)]->balance)
				node = rotate_double(stack[i], OTHERNODE(CMP2NODE(stack[i]->balance)));
			else
				node = rotate_single(stack[i], OTHERNODE(CMP2NODE(stack[i]->balance)));

			if(i == 0)
				root->node = node;
			else
				stack[i-1]->child[dir[i-1]] = node;

			stack[i] = node;
		}

		if(stack[i]->balance != 0)
			break;
	}

	*retval = avltree_node_init(retval->ref);
	root->count--;

	return retval;
}


/**
 * Initialize a node.
 *   @ref: The reference.
 *   &returns: The node.
 */

_export
struct avltree_node_t avltree_node_init(void *ref)
{
	return (struct avltree_node_t){ 0, ref, NULL, { NULL, NULL } };
}


/**
 * Retrieve the previous node.
 *   @node: The current node.
 *   &returns: The previous node or null.
 */

_export
struct avltree_node_t *avltree_node_prev(struct avltree_node_t *node)
{
	if(node->child[LEFT] != NULL) {
		node = node->child[LEFT];

		while(node->child[RIGHT] != NULL)
			node = node->child[RIGHT];

		return node;
	}
	else {
		while(node->parent != NULL) {
			if(node->parent->child[RIGHT] == node)
				break;

			node = node->parent;
		}

		return node->parent;
	}
}

/**
 * Retrieve the next node.
 *   @node: The current node.
 *   &returns: The next node or null.
 */

_export
struct avltree_node_t *avltree_node_next(struct avltree_node_t *node)
{
	if(node->child[RIGHT] != NULL) {
		node = node->child[RIGHT];

		while(node->child[LEFT] != NULL)
			node = node->child[LEFT];

		return node;
	}
	else {
		while(node->parent != NULL) {
			if(node->parent->child[LEFT] == node)
				break;

			node = node->parent;
		}

		return node->parent;
	}
}


/**
 * Initialize an AVL tree.
 *   @compare: The compare function.
 *   @delete: The deletion function.
 *   &returns: The AVL tree.
 */

_export
struct avltree_t avltree_init(compare_f compare, delete_f delete)
{
	return (struct avltree_t){ avltree_root_init(compare), delete };
}

/**
 * Destroy an AVL tree.
 *   @tree: The tree.
 */

_export
void avltree_destroy(struct avltree_t *tree)
{
	avltree_root_destroy(&tree->root, offsetof(struct avltree_inst_t, node), (delete_f)inst_delete);
}


/**
 * Lookup a key in the AVL tree.
 *   @tree: The tree.
 *   @key: The key.
 *   &returns: The reference if found, null otherwise.
 */

_export
void *avltree_lookup(struct avltree_t *tree, const void *key)
{
	struct avltree_node_t *node;

	node = avltree_root_lookup(&tree->root, key);
	return node ? getcontainer(node, struct avltree_inst_t, node) : NULL;
}

/**
 * Insert a key-value pair into the tree.
 *   @tree: The tree.
 *   @key: The key.
 *   @ref: The reference value.
 */

_export
void avltree_insert(struct avltree_t *tree, const void *key, void *ref)
{
	avltree_root_insert(&tree->root, &inst_new(key, ref, tree->delete)->node);
}


/**
 * Retrieve the first instance from the AVL tree.
 *   @tree: The AVL tree.
 *   &returns: The instance or null.
 */

_export
struct avltree_inst_t *avltree_first(struct avltree_t *tree)
{
	struct avltree_node_t *node;

	node = avltree_root_first(&tree->root);
	return node ? getcontainer(node, struct avltree_inst_t, node) : NULL;
}

/**
 * Retrieve the last instance from the AVL tree.
 *   @tree: The AVL tree.
 *   &returns: The instance or null.
 */

_export
struct avltree_inst_t *avltree_last(struct avltree_t *tree)
{
	struct avltree_node_t *node;

	node = avltree_root_last(&tree->root);
	return node ? getcontainer(node, struct avltree_inst_t, node) : NULL;
}

/**
 * Retrieve the previous AVL tree instance.
 *   @inst: The current instance.
 *   &returns: The previous instance or null.
 */

_export
struct avltree_inst_t *avltree_prev(struct avltree_inst_t *inst)
{
	struct avltree_node_t *node;

	node = avltree_node_prev(&inst->node);
	return node ? getcontainer(node, struct avltree_inst_t, node) : NULL;
}

/**
 * Retrieve the next AVL tree instance.
 *   @inst: The current instance.
 *   &returns: The next instance or null.
 */

_export
struct avltree_inst_t *avltree_next(struct avltree_inst_t *inst)
{
	struct avltree_node_t *node;

	node = avltree_node_next(&inst->node);
	return node ? getcontainer(node, struct avltree_inst_t, node) : NULL;
}


/**
 * Retrieve the reference from a node.
 *   @node: The node.
 *   @offset: The offset.
 *   &returns: The reference.
 */

static void *getref(struct avltree_node_t *node, ssize_t offset)
{
	return (void *)node - offset;
}


/**
 * Performs a single tree rotation of the given node. The node's child in the
 * opposite direction as the 'dir' paramter will replace itself as the parent,
 * placing the old parent as a child in the direction of the 'dir' parameter.
 * Wikipedia provides a good explanation with pictures.
 *   @node: The AVL tree node.
 *   @dir: The direction to rotate, should be either the value 'LEFT' or
 *     'RIGHT'.
 *   &returns: The node that now takes the place of the node that was passed
 *     in.
 */

static struct avltree_node_t *rotate_single(struct avltree_node_t *node, uint8_t dir)
{
	struct avltree_node_t *tmp;

	tmp = node->child[OTHERNODE(dir)];
	node->child[OTHERNODE(dir)] = tmp->child[dir];
	tmp->child[dir] = node;

	node->balance += NODEDIR(dir);
	if(NODEDIR(dir) * tmp->balance < 0)
		node->balance -= tmp->balance;

	tmp->balance += NODEDIR(dir);
	if(NODEDIR(dir) * node->balance > 0)
		tmp->balance += node->balance;

	tmp->parent = node->parent;
	node->parent = tmp;

	if(node->child[OTHERNODE(dir)] != NULL)
		node->child[OTHERNODE(dir)]->parent = node;

	return tmp;
}

/**
 * Performs a double rotation that is used to bring the grandchild to replace
 * its current position. Wikipedia provides a good explanation with pictures.
 *   @node: The AVL tree node.
 *   @dir: The direction to rotate, should be either the value 'LEFT' or
 *     'RIGHT'.
 *   &returns: The node that now takes the place of the node that was passed
 *     in.
 */

static struct avltree_node_t *rotate_double(struct avltree_node_t *node, uint8_t dir)
{
	node->child[OTHERNODE(dir)] = rotate_single(node->child[OTHERNODE(dir)], OTHERNODE(dir));

	return rotate_single(node, dir);
}


/**
 * Create an instance.
 *   @key: The key.
 *   @ref: The reference.
 *   @delete: Deletion.
 *   &returns: The instance.
 */

static struct avltree_inst_t *inst_new(const void *key, void *ref, delete_f delete)
{
	struct avltree_inst_t *inst;

	inst = mem_alloc(sizeof(struct avltree_inst_t));
	inst->node = avltree_node_init((void *)key);
	inst->ref = ref;
	inst->delete = delete;

	return inst;
}

/**
 * Delete an instance.
 *   @inst: The instance.
 */

static void inst_delete(struct avltree_inst_t *inst)
{
	inst->delete(inst->ref);
	mem_free(inst);
}


/**
 * Create an AVL tree from a constant string list.
 *   @list: The list.
 *   &returns: The tree.
 */

_export
struct avltree_t avltree_strlist(const char *const *list)
{
	struct avltree_t tree;

	tree = avltree_init(compare_str, delete_noop);

	for(; *list != NULL; list++)
		avltree_insert(&tree, *list, (void *)*list);

	return tree;
}
