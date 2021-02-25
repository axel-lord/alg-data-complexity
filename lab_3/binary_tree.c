#include "binary_tree.h"
#include <stdlib.h>
#include <stdio.h>

// static functions

// comparison returns 1 if val_2 >= val_1, 0 otherwise
static inline int greater_than_or_equal(bin_tree_data val_1, bin_tree_data val_2)
{
	return val_2 >= val_1;
}

// recursive part of free_binary_tree
static void free_node_and_children(TreeNode * node)
{
	if (node == NULL) return;
	free_node_and_children(node->left);
	free_node_and_children(node->right);
	free(node);
}

// recursive part of binary_tree_in_order_tree_walk
static void tree_node_in_order_tree_walk(TreeNode * node)
{
	if (node == NULL) return;
	tree_node_in_order_tree_walk(node->left);
	printf(BIN_TREE_DATA_STR_FORMAT " ", node->key);
	tree_node_in_order_tree_walk(node->right);
}

// gets minimum of subtree, returns null if given TreeNode is null
static TreeNode * subtree_minimum(TreeNode * node)
{
	if (node == NULL) return NULL;
	while(node->left != NULL)
		node = node->left;
	return node;
}

// gets maximum of subtree, returns null if given TreeNode is null
static TreeNode * subtree_maximum(TreeNode * node)
{
	if (node == NULL) return NULL;
	while(node->right != NULL)
		node = node->right;
	return node;
}

// subroutine to binary_tree_remove
static void tree_node_transplant(
		TreeNode * old_node, 
		TreeNode * new_node, 
		TreeNode ** parent_child_pointer
) {
	*parent_child_pointer = new_node;
	if (new_node != NULL)
		new_node->parent = old_node->parent;
}

// get depth of subtree
static int subtree_depth(TreeNode * node, int start_depth)
{
	if (node == NULL) return start_depth;
	start_depth++;
	int val_1 = subtree_depth(node->left, start_depth);
	int val_2 = subtree_depth(node->right, start_depth);
	return val_1 < val_2 ? val_2 : val_1;
}

// get size of subtree
static int subtree_size(TreeNode * node)
{
	if (node == NULL) return 0;
	return 1 + subtree_size(node->left) + subtree_size(node->right);
}

// implemented functions

BinaryTree * create_binary_tree()
{
	BinaryTree * tree = malloc(sizeof(BinaryTree));
	if (tree != NULL)
		tree->root = NULL;
	return tree;
}

void free_binary_tree(BinaryTree * tree)
{
	free_node_and_children(tree->root);
	free(tree);
}

TreeNode * create_tree_node(bin_tree_data key)
{
	TreeNode * node = malloc(sizeof(TreeNode));
	if (node != NULL)
	{
		node->key = key;
		node->parent = NULL;
		node->left = NULL;
		node->right = NULL;
	}
	return node;
}

void binary_tree_in_order_tree_walk(BinaryTree * tree)
{
	tree_node_in_order_tree_walk(tree->root);
}

TreeNode * binary_tree_search(BinaryTree * tree, bin_tree_data key)
{
	TreeNode * iter_node = tree->root;
	while (iter_node != NULL && iter_node->key != key)
	{
		if (greater_than_or_equal(iter_node->key, key))
			iter_node = iter_node->right;
		else
			iter_node = iter_node->left;
	}
	return iter_node;
}

int binary_tree_insert(BinaryTree * tree, TreeNode * node)
{
	if (node == NULL) // allows to be chained with functions that may return NULL
		return 0;

	if (tree->root == NULL) // if tree is empty
	{
		tree->root = node;
		return 1;
	}

	TreeNode * iter_node;
	TreeNode ** insert_after = &tree->root;
	while (*insert_after != NULL)
	{
		iter_node = *insert_after;
		if (greater_than_or_equal(iter_node->key, node->key))
			insert_after = &iter_node->right;
		else
			insert_after = &iter_node->left;
	}

	*insert_after = node;
	node->parent = iter_node;
	return 1;
}

TreeNode * binary_tree_remove(BinaryTree * tree, TreeNode * node)
{
	if (node == NULL) return NULL; // allows chaining with search

	// get pointer that points to node
	TreeNode ** parent_child_pointer;

	if (node == tree->root) // node is root node
		parent_child_pointer = &tree->root;
	else if (node->parent == NULL) // node is not root node but has no parent
		return NULL;
	else if (node == node->parent->right) // node is right child
		parent_child_pointer = &node->parent->right;
	else // node is left child 
		parent_child_pointer = &node->parent->left;
	
	// removal
	if (node->left == NULL)
		tree_node_transplant(node, node->right, parent_child_pointer);
	else if (node->right == NULL)
		tree_node_transplant(node, node->left, parent_child_pointer);
	else // node has two children
	{
		TreeNode * successor_node = subtree_minimum(node->right);
		if (successor_node->parent != node)
		{
			TreeNode ** s_parent_child_ptr;
			if (successor_node == successor_node->parent->right)
				s_parent_child_ptr = &successor_node->parent->right;
			else
				s_parent_child_ptr = &successor_node->parent->left;

			tree_node_transplant(successor_node, successor_node->right, s_parent_child_ptr);
			successor_node->right = node->right;
			successor_node->right->parent = successor_node;
		}
		tree_node_transplant(node, successor_node, parent_child_pointer);
		successor_node->left = node->left;
		successor_node->left->parent = successor_node;
	}

	// cleanup
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;

	return node;
}

TreeNode * binary_tree_minimum(BinaryTree * tree)
{
	return subtree_minimum(tree->root);
}

TreeNode * binary_tree_maximum(BinaryTree * tree)
{
	return subtree_maximum(tree->root);
}

TreeNode * binary_tree_successor(TreeNode * node)
{
	if (node == NULL) return NULL;
	if (node->right != NULL)
		return subtree_minimum(node->right);

	TreeNode * parent_iter_node = node->parent;
	while (parent_iter_node != NULL && node == parent_iter_node->right)
	{
		node = parent_iter_node;
		parent_iter_node = parent_iter_node->parent;
	}
	return parent_iter_node;
}

TreeNode * binary_tree_predecessor(TreeNode * node)
{
	if (node == NULL) return NULL;
	if (node->left != NULL)
		return subtree_minimum(node->left);

	TreeNode * parent_iter_node = node->parent;
	while (parent_iter_node != NULL && node == parent_iter_node->left)
	{
		node = parent_iter_node;
		parent_iter_node = parent_iter_node->parent;
	}
	return parent_iter_node;
}

int binary_tree_depth(BinaryTree * tree)
{
	return subtree_depth(tree->root, 0);
}

int binary_tree_size(BinaryTree * tree)
{
	return subtree_size(tree->root);
}
