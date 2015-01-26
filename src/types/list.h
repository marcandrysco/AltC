#ifndef TYPES_LIST_H
#define TYPES_LIST_H

/**
 * List root structure.
 *   @len: The length.
 *   @head, tail: The head and tail nodes.
 */

struct list_root_t {
	unsigned int len;
	struct list_node_t *head, *tail;
};

/**
 * List node structure.
 *   @prev, next: The previous and next node.
 */

struct list_node_t {
	struct list_node_t *prev, *next;
};


/**
 * Root chunk information structure.
 *   @root: The root.
 *   @offset: The reference offset.
 *   @init: The chunk initializer.
 *   @sep: The seperator.
 */

struct list_root_chunkinfo_t {
	struct list_root_t *root;
	ssize_t offset;
	io_chunk_init_f init;
	struct io_chunk_t sep;
};


/*
 * list root function declarations
 */

struct list_root_t list_root_init(void);
void list_root_destroy(struct list_root_t *root, ssize_t offset, delete_f delete);

void list_root_prepend(struct list_root_t *root, struct list_node_t *node);
void list_root_append(struct list_root_t *root, struct list_node_t *node);
void list_root_insert_before(struct list_root_t *root, struct list_node_t *cur, struct list_node_t *node);
void list_root_insert_after(struct list_root_t *root, struct list_node_t *cur, struct list_node_t *node);
void list_root_remove(struct list_root_t *root, struct list_node_t *node);

void list_root_print(struct list_root_t *root, struct io_output_t output, ssize_t offset, io_chunk_init_f init, struct io_chunk_t sep);
struct io_chunk_t list_root_chunk(const struct list_root_chunkinfo_t *info);

/*
 * convenience macros
 */

#define list_root_chunkval(root, offset, init, sep) list_root_chunk(&(struct list_root_chunkinfo_t){ root, offset, init, sep })


/**
 * Retrieve the reference from a node using an offset.
 *   @node: The node.
 *   @offset: The offset.
 *   &returns: The reference.
 */

static inline void *list_node_ref(struct list_node_t *node, ssize_t offset)
{
	return (void *)node - offset;
}

#endif
