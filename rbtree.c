/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

#include <rbtree.h>

/* --------------------------------------------------------------------------
 * LOCAL FUNCTIONS
 * -------------------------------------------------------------------------- */

static int _rbtree_rotate_left(struct rbtree *tree, struct rbtree_node *entry)
{
	struct rbtree_node *y;

	if (!tree || !tree->root || !entry)
		return -EINVAL;

	y = entry->right;
	entry->right = y->left;

	if (y->left)
		y->left->parent = entry;

	y->parent = entry->parent;

	if (!entry->parent) {
		tree->root = y;
	} else {
		if (entry == entry->parent->left)
			entry->parent = y;
		else
			entry->parent->right = y;
	}

	y->left = entry;
	entry->parent = y;

	return 0;
}

static int _rbtree_rotate_right(struct rbtree *tree, struct rbtree_node *entry)
{
	struct rbtree_node *x;

	if (!tree || !tree->root || !entry)
		return -EINVAL;

	x = entry->left;
	entry->left = x->right;

	if (x->right)
		x->left->parent = entry;

	if (!entry->parent) {
		tree->root = entry;
	} else {
		if (entry == entry->parent->right)
			entry->parent = x;
		else
			entry->parent->left = x;
	}

	x->right = entry;
	entry->parent = x;

	return 0;
}

static int _rbtree_insert(struct rbtree *tree, struct rbtree_node *entry)
{
	struct rbtree_node *x, *y;

	if (!tree || !rbtree_node_init(entry))
		return -EINVAL;

	y = NULL;
	x = tree->root;

	while (x) {
		int cmp;

		y = x;

		cmp = tree->keycmp(entry, x);
		if (cmp == 0)
			return -EALREADY;

		if (cmp < 0)
			x = x->left;
		else
			x = x->right;
	}

	entry->parent = y;
	if (!y) {
		tree->root = entry;
	} else {
		int cmp;

		cmp = tree->keycmp(entry, y);
		if (cmp < 0)
			y->left = entry;
		else
			y->right = entry;
	}

	return 0;
}

static int _rbtree_remove_fixup(struct rbtree *tree, struct rbtree_node *entry)
{
	struct rbtree_node *x, *w;
	int err;

	if (!tree || !entry)
		return -EINVAL;

	x = entry;

	while (x != tree->root && x->color == RBTREE_NODE_COLOR_BLACK) {
		if (x == x->parent->right) {
			w = x->parent->right;
			if (w->color == RBTREE_NODE_COLOR_RED) {
				w->color = RBTREE_NODE_COLOR_BLACK;
				x->parent->color = RBTREE_NODE_COLOR_RED;
				err = _rbtree_rotate_left(tree, x->parent);
				if (err < 0)
					return err;

				w = x->parent->right;
			}

			if (w->left->color == RBTREE_NODE_COLOR_BLACK && w->right->color == RBTREE_NODE_COLOR_BLACK) {
				w->color = RBTREE_NODE_COLOR_RED;
				x = x->parent;
			} else {
				if (w->right->color == RBTREE_NODE_COLOR_BLACK) {
					w->left->color = RBTREE_NODE_COLOR_BLACK;
					w->color = RBTREE_NODE_COLOR_RED;
					err = _rbtree_rotate_right(tree, w);
					if (err < 0)
						return err;
				}

				w->color = x->parent->color;
				x->parent->color = RBTREE_NODE_COLOR_BLACK;
				w->right->color = RBTREE_NODE_COLOR_BLACK;
				err = _rbtree_rotate_left(tree, x->parent);
				if (err < 0)
					return err;

				x = tree->root;
			}
		} else {
			w = x->parent->left;
			if (w->color == RBTREE_NODE_COLOR_RED) {
				w->color = RBTREE_NODE_COLOR_BLACK;
				x->parent->color = RBTREE_NODE_COLOR_RED;
				err = _rbtree_rotate_right(tree, x->parent);
				if (err < 0)
					return err;

				w = x->parent->left;
			}

			if (w->left->color == RBTREE_NODE_COLOR_BLACK && w->right->color == RBTREE_NODE_COLOR_BLACK) {
				w->color = RBTREE_NODE_COLOR_RED;
				x = x->parent;
			} else {
				if (w->left->color == RBTREE_NODE_COLOR_BLACK) {
					w->right->color = RBTREE_NODE_COLOR_BLACK;
					w->color = RBTREE_NODE_COLOR_RED;
					err = _rbtree_rotate_left(tree, w);
					if (err < 0)
						return err;
				}

				w->color = x->parent->color;
				x->parent->color = RBTREE_NODE_COLOR_BLACK;
				w->left->color = RBTREE_NODE_COLOR_BLACK;
				err = _rbtree_rotate_right(tree, x->parent);
				if (err < 0)
					return err;

				x = tree->root;
			}
		}
	}

	x->color = RBTREE_NODE_COLOR_BLACK;

	return 0;
}

static struct rbtree_node *_rbtree_min(struct rbtree_node *entry)
{
	if (!entry)
		return NULL;

	while (entry->left)
		entry = entry->left;

	return entry;
}

static struct rbtree_node *_rbtree_max(struct rbtree_node *entry)
{
	if (!entry)
		return NULL;

	while (entry->right)
		entry = entry->right;

	return entry;
}

/* --------------------------------------------------------------------------
 * API
 * -------------------------------------------------------------------------- */

int rbtree_insert(struct rbtree *tree, struct rbtree_node *entry)
{
	struct rbtree_node *x = entry, *y;
	int err;

	if (!tree || !entry)
		return -EINVAL;

	err = _rbtree_insert(tree, entry);
	if (err < 0)
		return err;

	while (x != tree->root && x->parent->color == RBTREE_NODE_COLOR_RED) {
		if (x->parent == x->parent->parent->right) {
			y = x->parent->parent->right;
			if (y->color == RBTREE_NODE_COLOR_RED) {
				x->parent->color = RBTREE_NODE_COLOR_BLACK;
				y->color = RBTREE_NODE_COLOR_BLACK;
				x->parent->parent->color = RBTREE_NODE_COLOR_RED;
				x = x->parent->parent;
			} else {
				if (x == x->parent->right) {
					x = x->parent;
					err = _rbtree_rotate_left(tree, x);
					if (err < 0)
						return err;
				}

				x->parent->color = RBTREE_NODE_COLOR_BLACK;
				x->parent->parent->color = RBTREE_NODE_COLOR_RED;
				err = _rbtree_rotate_right(tree, x->parent->parent);
				if (err < 0)
					return err;
			}
		} else {
			y = x->parent->parent->left;
			if (y->color == RBTREE_NODE_COLOR_RED) {
				x->parent->color = RBTREE_NODE_COLOR_BLACK;
				y->color = RBTREE_NODE_COLOR_BLACK;
				x->parent->parent->color = RBTREE_NODE_COLOR_RED;
				x = x->parent->parent;
			} else {
				if (x == x->parent->left) {
					x = x->parent;
					err = _rbtree_rotate_right(tree, x);
					if (err < 0)
						return err;
				}

				x->parent->color = RBTREE_NODE_COLOR_BLACK;
				x->parent->parent->color = RBTREE_NODE_COLOR_RED;
				err = _rbtree_rotate_left(tree, x->parent->parent);
				if (err < 0)
					return err;
			}
		}
	}

	tree->root->color = RBTREE_NODE_COLOR_BLACK;
	return 0;
}

int rbtree_remove(struct rbtree *tree, struct rbtree_node *entry)
{
	struct rbtree_node *x, *y;
	int err = 0;

	if (!tree || !entry)
		return -EINVAL;

	if (!entry->left || !entry->right)
		y = entry;
	else
		y = rbtree_node_next(entry);

	if (y->left)
		x = y->left;
	else
		x = y->right;

	if (x)
		x->parent = y->parent;

	if (!y->parent) {
		tree->root = x;
	} else {
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	}

	if (y->color == RBTREE_NODE_COLOR_BLACK)
		err = _rbtree_remove_fixup(tree, x);

	return err;
}

struct rbtree_node *rbtree_node_prev(struct rbtree_node *entry)
{
	struct rbtree_node *y;

	if (!entry)
		return NULL;

	if (entry->left)
		return _rbtree_max(entry->left);

	y = entry->parent;
	while (y && entry == y->left) {
		entry = y;
		y = y->parent;
	}

	return y;
}

struct rbtree_node *rbtree_node_next(struct rbtree_node *entry)
{
	struct rbtree_node *y;

	if (!entry)
		return NULL;

	if (entry->right)
		return _rbtree_min(entry->right);

	y = entry->parent;
	while (y && entry == y->right) {
		entry = y;
		y = y->parent;
	}

	return y;
}

struct rbtree_node *rbtree_search(struct rbtree *tree, int (*search_func)(struct rbtree_node *entry))
{
	struct rbtree_node *x;
	int err;

	if (!tree || !search_func)
		return NULL;

	x = tree->root;

	while (x) {
		err = search_func(x);
		if (!err)
			break;

		if (err < 0)
			x = x->left;
		else
			x = x->right;
	}

	return x;
}

int rbtree_for_each_node(struct rbtree *tree, int (*apply_func)(struct rbtree_node *entry))
{
	struct rbtree_node *entry;
	int err;

	if (!tree || !apply_func)
		return -EINVAL;

	entry = _rbtree_min(tree->root);
	while (entry) {
		err = apply_func(entry);
		if (err < 0)
			return err;

		entry = rbtree_node_next(entry);
	}

	return 0;
}
