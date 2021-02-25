#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>

#define COL_RESET "\033[0m"
#define COL_YELLOW "\033[1;93m"
#define COL_RED "\033[1;91m"
#define COL_GREEN "\033[92m"
#define COL_BOLD "\033[1m"

void print_node(TreeNode * node)
{
	if (node == NULL)
		printf("NULL");
	else
		printf(BIN_TREE_DATA_STR_FORMAT, node->key);
}

void print_successor(BinaryTree * tree, bin_tree_data key)
{
	int len = snprintf(NULL, 0, BIN_TREE_DATA_STR_FORMAT, key); //NOLINT
	char key_str [len+1];
	snprintf(key_str, len+1, BIN_TREE_DATA_STR_FORMAT, key); //NOLINT

	printf("\n" "%4s: ", key_str);
	print_node(binary_tree_successor(binary_tree_search(tree, key)));
}

void print_predecessor(BinaryTree * tree, bin_tree_data key)
{
	int len = snprintf(NULL, 0, BIN_TREE_DATA_STR_FORMAT, key); //NOLINT
	char key_str [len+1];
	snprintf(key_str, len+1, BIN_TREE_DATA_STR_FORMAT, key); //NOLINT

	printf("\n" "%4s: ", key_str);
	print_node(binary_tree_predecessor(binary_tree_search(tree, key)));
}

void print_key_width4(bin_tree_data key)
{
	int len = snprintf(NULL, 0, BIN_TREE_DATA_STR_FORMAT, key); //NOLINT
	char key_str [len+1];
	snprintf(key_str, len+1, BIN_TREE_DATA_STR_FORMAT, key); //NOLINT

	printf("%4s", key_str);
}

int main()
{
	BinaryTree * tree = create_binary_tree();
	if (tree == NULL)
		return 1;
	int numbers [] = {5, 0, 3, -3, 10, 15, 7, 13, 8};
	int number_count = sizeof(numbers) / sizeof(numbers[0]);
	
	printf(COL_BOLD "Array:" COL_RESET "\n");
	
	// testing insert
	for (int i = 0; i < number_count; ++i)
	{
		TreeNode * node = create_tree_node(numbers[i]);
		if (node == NULL)
		{
			free_binary_tree(tree);
			return 1;
		}
		binary_tree_insert(tree, node);
		printf("%d ", numbers[i]);
	}

	//testing depth and size
	int tree_size = binary_tree_size(tree);
	int tree_depth = binary_tree_depth(tree);

	printf("\n" COL_BOLD "Size:" COL_RESET " %d" "\n\n", number_count);
	
	printf(COL_BOLD "Tree Walk" COL_RESET "\n");
	binary_tree_in_order_tree_walk(tree);
	printf(
		"\n" 
		COL_BOLD "Size:" COL_RESET " %d, " 
		COL_BOLD "Depth:" COL_RESET " %d" 
		"\n\n", tree_size, tree_depth
	);
	
	//testing search and remove
	free(binary_tree_remove(tree, binary_tree_search(tree, 13)));
	free(binary_tree_remove(tree, binary_tree_search(tree,  7)));
	free(binary_tree_remove(tree, binary_tree_search(tree,  0)));

	tree_size = binary_tree_size(tree);
	tree_depth = binary_tree_depth(tree);

	printf(COL_BOLD "Removed:" COL_RESET " %d, %d, %d" "\n", 13, 7, 0);

	printf(COL_BOLD "Tree Walk" COL_RESET "\n");
	binary_tree_in_order_tree_walk(tree);
	printf(
		"\n" 
		COL_BOLD "Size:" COL_RESET " %d, " 
		COL_BOLD "Depth:" COL_RESET " %d" 
		"\n\n", tree_size, tree_depth
	);
	printf(
		COL_BOLD 
		"root-right-left:" 
		COL_RESET 
		"\n" 
		"  Expected: " BIN_TREE_DATA_STR_FORMAT "\n" 
		"       Key: " BIN_TREE_DATA_STR_FORMAT "\n",
		(bin_tree_data) 8, tree->root->right->left->key
	);
	printf(
		COL_BOLD 
		"root-right-right:" 
		COL_RESET 
		"\n" 
		"  Expected: " BIN_TREE_DATA_STR_FORMAT "\n" 
		"       Key: " BIN_TREE_DATA_STR_FORMAT "\n\n",
		(bin_tree_data) 15, tree->root->right->right->key
	);
	
	binary_tree_in_order_tree_walk(tree);
	printf(
		"\n" 
		COL_BOLD "Maximum:" COL_RESET " " BIN_TREE_DATA_STR_FORMAT ", " 
		COL_BOLD "Minimum:" COL_RESET " " BIN_TREE_DATA_STR_FORMAT "\n",
		binary_tree_maximum(tree)->key, 
		binary_tree_minimum(tree)->key
	);

	// test successor
	printf(
		COL_BOLD "Successor of:" COL_RESET
	);
	print_successor(tree, -3);
	print_successor(tree,  5);
	print_successor(tree, 15);
	
	//test Predecessor
	printf(
		"\n" COL_BOLD "Predecessor of:" COL_RESET
	);
	print_predecessor(tree, -3);
	print_predecessor(tree,  5);
	print_predecessor(tree, 15);

	printf("\n\n" COL_BOLD "Root removal:" "\n");
	printf("Removed Root:" COL_RESET " None, " COL_BOLD "Tree:" COL_RESET " ");
	binary_tree_in_order_tree_walk(tree);
	while (tree->root != NULL)
	{
		TreeNode * removed_node = binary_tree_remove(tree, tree->root);

		printf(
			"\n" COL_BOLD 
			"Removed Root:" 
			COL_RESET " "
		); 
		print_key_width4(removed_node->key);
		printf(
			", "
			COL_BOLD 
			"Tree:" 
			COL_RESET " "
		);
		binary_tree_in_order_tree_walk(tree);

		free(removed_node);
	}

	printf("\n");
	free_binary_tree(tree);
	return 0;
}
