#include <stdio.h>

#include "../rbtree.h"

struct testnode {
	int key;
	struct rbtree_node node;
};

int testnode_keycmp(struct rbtree_node *node1, struct rbtree_node *node2)
{
	struct testnode *elmt1, *elmt2;

	elmt1 = rbtree_node_entry(node1, struct testnode, node);
	elmt2 = rbtree_node_entry(node2, struct testnode, node);

	return elmt1->key - elmt2->key;
}

int testnode_print(struct rbtree_node *node)
{
	struct testnode *elmt;

	elmt = rbtree_node_entry(node, struct testnode, node);
	printf("%d ", elmt->key);

	return 0;
}

int main(int argc, char *argv[])
{
	struct rbtree t = RBTREE_INIT(testnode_keycmp);
	struct testnode node[10] = {
		{
			.key = 0,
		},
		{
			.key = 9,
		},
		{
			.key = 8,
		},
		{
			.key = 7,
		},
		{
			.key = 2,
		},
		{
			.key = 5,
		},
		{
			.key = 4,
		},
		{
			.key = 1,
		},
		{
			.key = 3,
		},
		{
			.key = 6,
		},
	};

	printf("INSERT: rbtree: ");
	rbtree_insert(&t, &node[0].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("INSERT: rbtree: ");
	rbtree_insert(&t, &node[1].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("INSERT: rbtree: ");
	rbtree_insert(&t, &node[2].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("INSERT: rbtree: ");
	rbtree_insert(&t, &node[3].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("INSERT: rbtree: ");
	rbtree_insert(&t, &node[4].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("INSERT: rbtree: ");
	rbtree_insert(&t, &node[5].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("INSERT: rbtree: ");
	rbtree_insert(&t, &node[6].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("INSERT: rbtree: ");
	rbtree_insert(&t, &node[7].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("INSERT: rbtree: ");
	rbtree_insert(&t, &node[8].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("INSERT: rbtree: ");
	rbtree_insert(&t, &node[9].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("REMOVE: rbtree: ");
	rbtree_remove(&t, &node[9].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("REMOVE: rbtree: ");
	rbtree_remove(&t, &node[8].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("REMOVE: rbtree: ");
	rbtree_remove(&t, &node[7].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("REMOVE: rbtree: ");
	rbtree_remove(&t, &node[6].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("REMOVE: rbtree: ");
	rbtree_remove(&t, &node[5].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("REMOVE: rbtree: ");
	rbtree_remove(&t, &node[4].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("REMOVE: rbtree: ");
	rbtree_remove(&t, &node[3].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("REMOVE: rbtree: ");
	rbtree_remove(&t, &node[2].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("REMOVE: rbtree: ");
	rbtree_remove(&t, &node[1].node);
	rbtree_for_each_node(&t, testnode_print);
	printf("\n");

	printf("REMOVE: rbtree: ");
	rbtree_remove(&t, &node[0].node);
	rbtree_for_each_node(&t, testnode_print);
	if (rbtree_is_empty(&t))
		printf("empty rbtree");
	printf("\n");

	return 0;
}
