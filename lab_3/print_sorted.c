#include "print_sorted.h"
#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>

int print_sorted(int size, int * array)
{
	BinaryTree * tree = create_binary_tree();
	if (tree == NULL)
		return 0;

	for (int i = 0; i < size; ++i)
	{
		TreeNode * node = create_tree_node(array[i]);
		if (node == NULL)
		{
			free_binary_tree(tree);
			return 0;
		}
		binary_tree_insert(tree, node);
	}

	binary_tree_in_order_tree_walk(tree);
	free_binary_tree(tree);
	printf("\n");

	return 1;
}
