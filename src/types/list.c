#include "../common.h"
#include "list.h"
#include "../io/chunk.h"


/*
 * local function declarations
 */

static void root_chunk(struct io_output_t output, void *arg);


/**
 * Initialize a list root.
 *   &returns: The list root.
 */

_export
struct list_root_t list_root_init(void)
{
	return (struct list_root_t){ 0, NULL, NULL };
}

/**
 * Destroy a list root.
 *   @root: The list root.
 *   @offset; The offset.
 *   @delete; The deletion callback.
 */

_export
void list_root_destroy(struct list_root_t *root, ssize_t offset, delete_f delete)
{
	struct list_node_t *cur, *next;

	cur = root->head;
	while(cur != NULL) {
		next = cur->next;
		delete((void *)cur - offset);
		cur = next;
	}
}


/**
 * Prepend a node onto the list root.
 *   @root: The root.
 *   @node: The node.
 */

_export
void list_root_prepend(struct list_root_t *root, struct list_node_t *node)
{
	node->next = root->head;
	node->prev = NULL;

	if(root->head != NULL)
		root->head->prev = node;
	else
		root->tail = node;

	root->len++;
	root->head = node;
}

/**
 * Append a node onto the list root.
 *   @root: The root.
 *   @node: The node.
 */

_export
void list_root_append(struct list_root_t *root, struct list_node_t *node)
{
	node->prev = root->tail;
	node->next = NULL;

	if(root->tail != NULL)
		root->tail->next = node;
	else
		root->head = node;

	root->len++;
	root->tail = node;
}


/**
 * Insert a node onto the list root before a cursor.
 *   @root: The root.
 *   @cur: The cursor node.
 *   @node: The node.
 */

_export
void list_root_insert_before(struct list_root_t *root, struct list_node_t *cur, struct list_node_t *node)
{
	if(cur->prev != NULL)
		cur->prev->next = node;
	else
		root->head = node;

	node->next = cur;
	node->prev = cur->prev;
	cur->prev = node;

	root->len++;
}

/**
 * Insert a node onto the list root after a cursor.
 *   @root: The root.
 *   @cur: The cursor node.
 *   @node: The node.
 */

_export
void list_root_insert_after(struct list_root_t *root, struct list_node_t *cur, struct list_node_t *node)
{
	if(cur->next != NULL)
		cur->next->prev = node;
	else
		root->tail = node;

	node->prev = cur;
	node->next = cur->next;
	cur->next = node;

	root->len++;
}

/**
 * Remove a node from the root.
 *   @root: The root.
 *   @node: The node.
 */

_export
void list_root_remove(struct list_root_t *root, struct list_node_t *node)
{
	if(node->next == NULL)
		root->tail = node->prev;
	else
		node->next->prev = node->prev;

	if(node->prev == NULL)
		root->head = node->next;
	else
		node->prev->next = node->next;

	root->len--;
}


/**
 * Print a list of items.
 *   @root: The root.
 *   @output: Theoutput.
 *   @offset; The node offset.
 *   @init: The chunk initializer.
 *   @sep: The seperator.
 */

_export
void list_root_print(struct list_root_t *root, struct io_output_t output, ssize_t offset, io_chunk_init_f init, struct io_chunk_t sep)
{
	struct list_node_t *node;

	for(node = root->head; node != NULL; node = node->next) {
		if(node != root->head)
			io_chunk_proc(sep, output);

		io_chunk_proc(init(list_node_ref(node, offset)), output);
	}
}

/**
 * Create an I/O chunk for the list.
 *   @info: The printing information.
 *   &returns: The chunk.
 */

_export
struct io_chunk_t list_root_chunk(const struct list_root_chunkinfo_t *info)
{
	return (struct io_chunk_t){ root_chunk, (void *)info };
}

/**
 * Process a root chunk.
 *   @output: The output.
 *   @arg: The argument.
 */

static void root_chunk(struct io_output_t output, void *arg)
{
	const struct list_root_chunkinfo_t *info = arg;

	list_root_print(info->root, output, info->offset, info->init, info->sep);
}
