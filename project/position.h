#ifndef PROJECT_POSITION_H
#define PROJECT_POSITION_H

#include <stdlib.h>

/* type to store positions */
typedef struct position_s
{
	size_t x;
	size_t y;
} 
position;

/* test if two positions are equal */
int position_equal(position p1, position p2);

/* type to store a queue of positions */
typedef struct position_queue_s position_queue;

/* type to store a stack of positions */
typedef struct position_stack_s position_stack;

/* create a queue for positions, returns NULL on failure */
position_queue * create_position_queue();

/* create a stack for positions, returns NULL on failure */
position_stack * create_position_stack();

/* free a position queue */
void free_position_queue(position_queue * q);

/* free a position stack */
void free_position_stack(position_stack * s);

/* enqueue a position, returns 0 on failure 1 otherwise */
int position_enqueue(position_queue * q, position pos);

/* push a position, returns 0 on failure 1 otherwise */
int position_push(position_stack * s, position pos);

/* dequeue a position, write to pos, returns 0 if empty 1 otherwise */
int position_dequeue(position_queue * q, position * pos);

/* pop a position, write to pos, returns 0 if empty 1 otherwise */
int position_pop(position_stack * s, position * pos);

/* see if a queue is empty, returns 1 if true else 0*/
int position_queue_is_empty(position_queue * q);

/* see if a stack is empty, returns 1 if true else 0*/
int position_stack_is_empty(position_stack * s);

/* build a position path/stack from a prev-matrix, 
 * Returns NULL on failure a position stack otherwise */
position_stack * build_path_stack(
	position source, 
	position dest, 
	size_t rows, 
	size_t columns, 
	position prev[rows][columns],
	double distance[rows][columns]
);

/* print the contents of a position stack without emptying it 
 * returns 0 if the stack is empty else 1
 */
int print_position_stack(position_stack * s);

#endif // PROJECT_POSITION_H
