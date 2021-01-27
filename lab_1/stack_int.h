#ifndef STACK_INT_HEADER
#define STACK_INT_HEADER

typedef struct int_stack_struct int_stack;

/* Returns:
 * a pointer to a new stack if succesfull
 * NULL, otherwise
 */
int_stack * create_int_stack();

/* free stack and all data */
void free_int_stack(int_stack * stack);

/* pushes given int onto stack
 * Parameters:
 * stack, stack to be pushed onto
 * value, iteger to be pushed
 * Returns:
 * 1, if push succesfull
 * 0, otherwise
 */
int int_stack_push(int_stack * stack, int value);

/* pops top element of stack and writes
 * it to given pointer
 * Parameters:
 * stack, stack to be popped from
 * return_int, int ptr to be written to
 * Returns:
 * 1, if pop was succesfull
 * 0, if stack is empty
 */
int int_stack_pop(int_stack * stack, int * return_int);

/* Returns 1 if given stack is empty else 1 */
int int_stack_empty(int_stack * stack);

#endif
