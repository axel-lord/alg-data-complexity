#include <stdio.h>
#include "queue_int.h"
#include "stack_int.h"

/* returns 1 if memory allocation fails somewhere 0 otherwise */
int main()
{
	int p_num [] = {2, 0, 0, 0, 0, 3, 0, 4};
	int p_length = sizeof(p_num)/sizeof(p_num[0]);
	int_stack * p_stack;
	int_queue * p_queue;

	p_stack = create_int_stack();
	if (p_stack == NULL) 
		return 1;
	p_queue = create_int_queue();
	if (p_queue == NULL)
	{
		free_int_stack(p_stack);
		return 1;
	}
	
	if (p_length)
	{
		printf("Adding number to stack: \n");
		// adding integers to stack
		for (int i = 0; i < p_length; ++i)
		{
			if (i > 0)
				printf(", ");
			printf("%d", p_num[i]);
			int result = int_stack_push(p_stack, p_num[i]);
			if (!result)
			{
				free_int_queue(p_queue);
				free_int_stack(p_stack);
				return 1;
			}
		}
		
		printf("\nPopping number from stack and adding to queue:\n");
		int pop_value = 0;
		while (int_stack_pop(p_stack, &pop_value))
		{
			printf("%d", pop_value);
			int_queue_enqueue(p_queue, pop_value);
			if (!int_stack_empty(p_stack))
				printf(", ");
		}

		printf("\nDequeueing number:\n");
		int dequeue_value = 0;
		while (int_queue_dequeue(p_queue, &dequeue_value))
		{
			printf("%d", dequeue_value);
			if (!int_queue_empty(p_queue))
				printf(", ");
		}
				
	}

	printf("\n");
	free_int_queue(p_queue);
	free_int_stack(p_stack);
	return 0;
}

