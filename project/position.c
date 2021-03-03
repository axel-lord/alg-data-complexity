#include "position.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct position_node_s
{
	position pos;
	struct position_node_s * next;
} position_node;

struct position_queue_s
{
	position_node * head;
	position_node * tail;
};

struct position_stack_s
{
	position_node * top;
};

position_queue * create_position_queue()
{
	position_queue * q = malloc(sizeof(position_queue));
	if (!q) return NULL;
	q->head = NULL;
	q->tail = NULL;
	return q;
}

position_stack * create_position_stack()
{
	position_stack * s = malloc(sizeof(position_stack));
	if (!s) return NULL;
	s->top = NULL;
	return s;
}

static void free_node_list(position_node * n)
{
	while(n)
	{
		position_node * del_node = n;
		n = n->next;
		free(del_node);
	}
}

void free_position_queue(position_queue * q)
{
	free_node_list(q->head);
	free(q);
}

void free_position_stack(position_stack * s)
{
	free_node_list(s->top);
	free(s);
}

int position_enqueue(position_queue * q, position pos)
{
	position_node * n = malloc(sizeof(position_node));
	if (!n) return 0;

	n->next = NULL;
	n->pos = pos;
	
	if (position_queue_is_empty(q))
		q->head = n;
	else
		q->tail->next = n;
	q->tail = n;
	return 1;
}

int position_push(position_stack * s, position pos)
{
	position_node * n = malloc(sizeof(position_node));
	if (!n) return 0;

	n->next = s->top;
	n->pos = pos;

	s->top = n;
	return 1;
}

int position_dequeue(position_queue * q, position * pos)
{
	if (position_queue_is_empty(q))
		return 0;

	position_node * n = q->head;
	q->head = n->next;
	*pos = n->pos;

	if (q->head == NULL)
		q->tail = NULL;

	free(n);
	return 1;
}

int position_pop(position_stack * s, position * pos)
{
	if (position_stack_is_empty(s))
		return 0;
	
	position_node * d = s->top;
	*pos = d->pos;
	s->top = d->next;
	free(d);
	return 1;
}

int position_queue_is_empty(position_queue * q)
{
	return q->head == NULL;
}

int position_stack_is_empty(position_stack * s)
{
	return s->top == NULL;
}

int position_equal(position p1, position p2)
{
	return p1.x == p2.x && p1.y == p2.y;
}

position_stack * build_path_stack(
	position source, 
	position dest, 
	size_t rows, 
	size_t columns, 
	position prev[rows][columns],
	double distance[rows][columns]
) {
	position_stack * path = create_position_stack();
	if (!path) return NULL;

	// macro lets me keep sane
#define S_PUSH(p) {if (!position_push(path, p)) {free_position_stack(path); return NULL;}}
	S_PUSH(dest)

	if (position_equal(source, dest))
		return path;

	if (isinf(distance[dest.y][dest.x]))
	{
		S_PUSH(source)
		return path;
	}
	
	position iter = prev[dest.y][dest.x];
	for (size_t i = 0; i < rows * columns; ++i) // prevents infinite loop
	{
		S_PUSH(iter)
		if (position_equal(iter, source))
			break;
		iter = prev[iter.y][iter.x];
	}
	
	return path;
#undef S_PUSH
}

int print_position_stack(position_stack * s)
{
	if (position_stack_is_empty(s))
	{
		printf("EMPTY");
		return 0;
	}
	position_node * iter = s->top;
	printf("(%zu,%zu)", iter->pos.x, iter->pos.y);
	iter = iter->next;
	while (iter != NULL)
	{
		printf("->(%zu,%zu)", iter->pos.x, iter->pos.y);
		iter = iter->next;
	}
	return 1;
}
