#include <stdlib.h>
#include "linked_list_int.h"
#include "stack_int.h"

struct int_stack_struct
{
	List * stack_data;
};

int_stack * create_int_stack()
{
	int_stack * n_stack = malloc(sizeof(int_stack));
	if (n_stack == NULL) return NULL;

	n_stack->stack_data = createList();
	if (n_stack->stack_data == NULL)
	{
		free(n_stack);
		return NULL;
	}

	return n_stack;
}

void free_int_stack(int_stack * stack)
{
	freeList(stack->stack_data);
	free(stack);
}

int int_stack_push(int_stack * stack, int value)
{
	Node * d_node = createNode(value);
	if (d_node == NULL)
		return 0;

	int result = insert(stack->stack_data, d_node);
	if (!result)
		free(d_node);

	return result;
}

int int_stack_pop(int_stack * stack, int * return_int)
{
	if (isEmpty(stack->stack_data))
		return 0;
	*return_int = stack->stack_data->NIL->next->data;
	free(delete(stack->stack_data, stack->stack_data->NIL->next));
	return 1;
}

int int_stack_empty(int_stack * stack)
{
	return isEmpty(stack->stack_data);
}

