#ifndef LAB_3_BINARY_TREE_H
#define LAB_3_BINARY_TREE_H

// as long as the keys are numbers below two lines can be changed to change type
#define BIN_TREE_DATA_STR_FORMAT "%d"
typedef int bin_tree_data;

typedef struct TreeNode_s
{
	bin_tree_data key;
	struct TreeNode_s * parent;
	struct TreeNode_s * left;
	struct TreeNode_s * right;
}
TreeNode;

typedef struct BinaryTree_s
{
	TreeNode * root;
}
BinaryTree;

/* create a BinaryTree
 * Returns:
 * NULL, if creation failed
 * a pointer to the new tree otherwise
 */
BinaryTree * create_binary_tree();

/* free a BinaryTree and all nodes
 * Parameters:
 * tree, BinaryTree to free
 */
void free_binary_tree(BinaryTree * tree);

/* create a TreeNode and set values to NULL
 * Parameters:
 * key, key to give node
 * Returns:
 * NULL, if creation of node failed
 * a pointer to the created TreeNode otherwise
 */
TreeNode * create_tree_node(bin_tree_data key);

/* Print tree contents in order of key size
 * Parameters:
 * tree, tree to print
 */
void binary_tree_in_order_tree_walk(BinaryTree * tree);

/* Search for node with given key
 * Parameters:
 * tree, a binary tree to search in
 * key, a key to search for
 * Returns:
 * NULL, if key is not found
 * a pointer to the node containing the key otherwise
 */
TreeNode * binary_tree_search(BinaryTree * tree, bin_tree_data key);

/* Insert a node into a tree
 * Parameters:
 * tree, tree to insert into;
 * node, node to insert
 * Returns:
 * 1, if insertion was succesfull
 * 0, otherwise
 */
int binary_tree_insert(BinaryTree * tree, TreeNode * node);

/* Remove a node from a tree
 * Parameters:
 * tree, tree to remove node from
 * node, node to remove
 * Returns:
 * NULL, if the node could not be removed
 * a pointer to the node otherwise
 */
TreeNode * binary_tree_remove(BinaryTree * tree, TreeNode * node);

/* Find the minimum node in a tree
 * Parameters:
 * tree, tree to search in
 * Returns:
 * NULL, if tree is empty
 * a pointer to the minimum node otherwise
 */
TreeNode * binary_tree_minimum(BinaryTree * tree);

/* Find the maximum node in a tree
 * Parameters:
 * tree, tree to search in
 * Returns:
 * NULL, if tree is empty
 * a pointer to the maximum node otherwise
 */
TreeNode * binary_tree_maximum(BinaryTree * tree);

/* Find the successor to a node in a tree
 * Parameters:
 * tree, tree to search in
 * node, node to find the successor of
 * Returns:
 * NULL, if given node has no successor
 * a pointer to the successor otherwise
 */
TreeNode * binary_tree_successor(TreeNode * node);

/* Find the predecessor to a node in a tree
 * Parameters:
 * tree, tree to search in
 * node, node to find the predecessor of
 * Returns:
 * NULL, if given node has no predecessor
 * a pointer to the predecessor otherwise
 */
TreeNode * binary_tree_predecessor(TreeNode * node);

/* Find the depth of a BinaryTree
 * Parameters:
 * tree, tree to find depth of
 * Returns:
 * tree depth
 */
int binary_tree_depth(BinaryTree * tree);

/* Find the size of a BinaryTree
 * Parameters:
 * tree, tree to find size of
 * Returns:
 * tree size
 */
int binary_tree_size(BinaryTree * tree);

#endif //LAB_3_BINARY_TREE_H
