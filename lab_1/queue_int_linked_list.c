#include <stdlib.h>
#include "linked_list_int.h"
#include "queue_int.h"

struct int_queue_s
{
	List * queue_data;
};

int_queue * create_int_queue()
{
	List * inner_list = createList();
	if (inner_list == NULL)
		return NULL;

	int_queue * Q = malloc(sizeof(int_queue));
	if (Q == NULL)
	{
		freeList(inner_list);
		return NULL;
	}

	Q->queue_data = inner_list;

	return Q;
}

void free_int_queue(int_queue * Q)
{
	freeList(Q->queue_data);
	free(Q);
}

int int_queue_enqueue(int_queue * Q, int value)
{
	Node * n_node = createNode(value);
	if (n_node == NULL)
		return 0;
	insert(Q->queue_data, n_node);
	return 1;
}

int int_queue_dequeue(int_queue * Q, int * return_int)
{
	if (isEmpty(Q->queue_data))
		return 0;
	*return_int = Q->queue_data->NIL->prev->data;
	free(delete(Q->queue_data, Q->queue_data->NIL->prev));
	return 1;
}

int int_queue_empty(int_queue * Q)
{
	return isEmpty(Q->queue_data);
}

