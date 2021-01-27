#include <stdio.h>
#include <stdlib.h>
#include "linked_list_int.h"

#define TEST_ARRAY_LENGHT 25
#define SEARCH_VAL 15

int main()
{
	List * int_list = createList();

	if (!isEmpty(int_list))
		printf("isEmpty on empty list failed");

	for (int i = 0; i < TEST_ARRAY_LENGHT; i++)
	{
		insert(int_list, createNode(i));
	}

	if (isEmpty(int_list))
		printf("isEmpty on non-empty list failed");

	// iter forwards
	Node * iter_node = int_list->NIL->next;
	for (int i = TEST_ARRAY_LENGHT - 1; i >= 0; --i)
	{
		if (iter_node->data != i)
			printf("Test nextlink/insert failure, list data: %d, correct value: %d\n", iter_node->data, i);
		iter_node = iter_node->next;
	}
	
	// iter backwards
	iter_node = int_list->NIL->prev;
	for (int i = 0; i < TEST_ARRAY_LENGHT; i++)
	{
		if (iter_node->data != i)
			printf("Test prevlink/insert failure, list data: %d, correct value: %d\n", iter_node->data, i);
		iter_node = iter_node->prev;
	}

	Node * node_s = search(int_list, 15);
	if (node_s->data != 15) 
		printf("Search test failed! data: %d, correct: %d", node_s->data, 15);

	Node * n_s_pred = predecessor(int_list, node_s);
	if (n_s_pred->data != SEARCH_VAL -1) 
		printf("Predecessor test failed! data: %d, correct: %d", n_s_pred->data, SEARCH_VAL -1);

	node_s->data = TEST_ARRAY_LENGHT + SEARCH_VAL;

	Node * n_s_pred_succ = successor(int_list, n_s_pred);
	if (n_s_pred_succ->data != SEARCH_VAL+1) 
		printf("Successor test failed! data: %d, correct: %d", n_s_pred_succ->data, SEARCH_VAL+1);

	Node * n_max = maximum(int_list);
	if (n_max != node_s)
		printf("maximum test failed! data: %d, correct: %d", n_max->data, node_s->data);

	n_s_pred->data = -SEARCH_VAL;

	Node * n_min = minimum(int_list);
	if (n_min != n_s_pred)
		printf("minimum test failed! data: %d, correct: %d", n_min->data, n_s_pred->data);

	free(delete(int_list, int_list->NIL->prev));
	if (int_list->NIL->prev->data != 1)
		printf("delete test failed! data: %d, correct: %d", int_list->NIL->prev->data, 1);

	freeList(int_list);
	return 0;
}
