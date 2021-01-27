#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

/* Node
 * a node with data belonging to a linked list
 *
 * Attributes:
 * data, an integer stored in the node
 * next, a pointer to the next node in the list, 
 *       NULL if node does not belong to a list
 * prev, a pointer to the previous element in the list
 *       NULL if node does not belong to a list
 */
typedef struct node_struct {
	int data;
	struct node_struct * next;
	struct node_struct * prev;
} Node;

/* List
 * a linked list storing integers
 * Attributes:
 * NIL, sentinel node NIL->next is the first node and
 *      NIL->prev is the last node
 */
typedef struct list_struct {
	Node * NIL;
} List;

/* create and retun a node (pointer)
 * Returns:
 * a Node pointer if creation successful
 * NULL otherwise;
 */
Node * createNode(int key);

/* create and return an empty list (pointer)
 * Returns;
 * a List pointer if creation successful
 * NULL otherwise
 */
List * createList();

/* free a list and all nodes belonging to it */
void freeList(List * L);

/* Parameters:
 * L, a list
 * Returns:
 * 1 (true), if the list L is empty
 * 0 (false), otherwise
 */
int isEmpty(List * L);

/* Insert a Node into a List
 * Parameters:
 * L, a list to be modified
 * N, a node to be inserted into the list
 * Returns:
 * 1, if insert was successful
 * 0, otherwise (ex. N is NULL)
 */
int insert(List * L, Node * N);

/* Parameters:
 * L, a list to search through
 * k, a key to search for
 * Returns:
 * a pointer to the node with data k if it exists
 * NULL, otherwise
 */
Node * search(List * L, int k);

/* Remove node N from list L
 * Parameters:
 * L, a list
 * N, a node to be removed from L
 * Returns:
 * N, if delete successful:
 * NULL, otherwise
 */
Node * delete(List * L, Node * N);

/* Parameters:
 * L, a list
 * Returns:
 * a pointer to the node with the largest key
 * NULL, if the list is empty
 */
Node * maximum(List * L);

/* Parameters:
 * L, a list
 * Returns:
 * a pointer to the node with the smallest key
 * NULL, if the list is empty
 */
Node * minimum(List * L);

/* Parameters:
 * L, a list
 * N, a node
 * Returns:
 * a pointer to the node with the next larger key to the give node
 * NULL, if the list is empty or if the node is the maximum node
 */
Node * successor(List * L, Node * N);

/* Parameters:
 * L, a list
 * N, a node
 * Returns:
 * a pointer to the node with the next smaller key to the give node
 * NULL, if the list is empty or if the node is the minimum node
 */
Node * predecessor(List * L, Node * N);

#endif
