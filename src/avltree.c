#include "common.h"
#include "avltree.h"


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

static struct avltree_node_t *rotate_single(struct avltree_node_t *node, uint8_t dir);
static struct avltree_node_t *rotate_double(struct avltree_node_t *node, uint8_t dir);


/**
 * Create an empty AVL tree root.
 *   &returns: The empty root.
 */

_export
struct avltree_root_t avltree_root_init()
{
	return (struct avltree_root_t){ NULL };
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
 *   @key: The sought key.
 *   @compare: The node-key comparison function.
 *   @arg: An argument passed to the comparison function.
 *   &returns: The node if found, null otherwise.
 */

_export
struct avltree_node_t *avltree_root_lookup(struct avltree_root_t *root, const void *key, avltree_nodekey_f compare, void *arg)
{
	int cmp;
	struct avltree_node_t *node = root->node;

	while(node != NULL) {
		cmp = compare(key, node, arg);
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
 *   @compare: The node-node comparison function.
 *   @arg: An argument passed to the comparison function.
 */

_export
void avltree_root_insert(struct avltree_root_t *root, struct avltree_node_t *node, avltree_nodenode_f compare, void *arg)
{
	int cmp;
	short i;
	uint8_t dir[AVLTREE_MAX_HEIGHT];
	struct avltree_node_t *stack[AVLTREE_MAX_HEIGHT];

	*node = AVLTREE_NODE_INIT;

	if(root->node == NULL) {
		root->node = node;
		node->parent = NULL;

		return;
	}

	stack[0] = root->node;

	for(i = 0; stack[i] != NULL && i < AVLTREE_MAX_HEIGHT; i++) {
		cmp = compare(node, stack[i], arg);
		if(cmp == 0)
			abort();

		dir[i] = CMP2NODE(cmp);
		stack[i+1] = stack[i]->child[dir[i]];
	}

	i--;
	stack[i]->child[dir[i]] = node;
	stack[i]->balance += NODEDIR(dir[i]);
	node->parent = stack[i];

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
 *   @key: The sought key.
 *   @compare: The node-key comparison function.
 *   &returns: The node if found, null otherwise.
 */

_export
struct avltree_node_t *avltree_root_remove(struct avltree_root_t *root, const void *key, avltree_nodekey_f compare, void *arg)
{
	int cmp;
	short i, ii;
	uint8_t dir[AVLTREE_MAX_HEIGHT];
	struct avltree_node_t *stack[AVLTREE_MAX_HEIGHT], *node, *retval;

	if(root->node == NULL)
		return NULL;

	stack[0] = root->node;

	for(i = 0; stack[i] != NULL && i < AVLTREE_MAX_HEIGHT; i++) {
		cmp = compare(key, stack[i], arg);
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

	return retval;
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
