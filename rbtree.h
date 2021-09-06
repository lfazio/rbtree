#ifndef RBTREE_H_
#define RBTREE_H_

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------------
 * TYPES
 * -------------------------------------------------------------------------- */

#define RBTREE_NODE_COLOR_BLACK false
#define RBTREE_NODE_COLOR_RED true

struct rbtree_node {
	bool color;
	struct rbtree_node *parent;
	struct rbtree_node *left, *right;
};

struct rbtree {
	struct rbtree_node *root;
	int (*keycmp)(struct rbtree_node *, struct rbtree_node *);
};

/* --------------------------------------------------------------------------
 * MACROS
 * -------------------------------------------------------------------------- */

#ifndef container_of
#define container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) \
    *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#define RBTREE_INIT(keycmp_func) { \
		.root = NULL, \
		.keycmp = keycmp_func, \
	}

#define RBTREE_NODE_INIT() { \
		.color = RBTREE_NODE_COLOR_BLACK, \
		.parent = NULL, \
		.left = NULL, \
		.right = NULL, \
	}

/* --------------------------------------------------------------------------
 * API
 * -------------------------------------------------------------------------- */

static inline struct rbtree *rbtree_init(struct rbtree *tree,
					 int (*keycmp_func)(struct rbtree_node *, struct rbtree_node *))
{
	if (!tree || keycmp_func)
		return NULL;

	tree->root = NULL;
	tree->keycmp = keycmp_func;

	return tree;
}

static inline struct rbtree_node *rbtree_node_init(struct rbtree_node *node)
{
	if (!node)
		return NULL;

	node->color = RBTREE_NODE_COLOR_BLACK;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;

	return node;
}

int rbtree_insert(struct rbtree *tree, struct rbtree_node *entry);
int rbtree_remove(struct rbtree *tree, struct rbtree_node *entry);
struct rbtree_node *rbtree_search(struct rbtree *tree, int (*search_func)(struct rbtree_node *entry));
int rbtree_for_each_node(struct rbtree *tree, int (*apply_func)(struct rbtree_node *entry));

#define rbtree_is_empty(tree) (!(tree)->root)
#define rbtree_node_entry(NODE, TYPE, MEMBER) container_of(NODE, TYPE, MEMBER)
struct rbtree_node *rbtree_node_prev(struct rbtree_node *entry);
struct rbtree_node *rbtree_node_next(struct rbtree_node *entry);

#endif /* !RBTREE_H_ */
