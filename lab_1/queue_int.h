#ifndef QUEUE_INT_HEADER
#define QUEUE_INT_HEADER

typedef struct int_queue_s int_queue;

/* Returns:
 * a new int_queue (pointer)
 * NULL if creation fails
 */
int_queue * create_int_queue();

/* Release all memory occupied by int_queue Q*/
void free_int_queue(int_queue * Q);

/* Enqueue an int to a queue
 * Parameters:
 * Q, queue to enqueue onto
 * value, value to enqueue
 * Returns:
 * 1, if enqueue succeeds
 * 0, otherwise
 */
int int_queue_enqueue(int_queue * Q, int value);

/* Dequeue an int from a queue
 * Parameters:
 * Q, queue to dequeue from
 * return_int, pointer to an integer to write dequeued value to
 * Returns:
 * 1, if dequeue succeeds
 * 0, if the queue is empty
 */
int int_queue_dequeue(int_queue * Q, int * return_int);

/* Returns 1 if given queue (Q) is empty else 0 */
int int_queue_empty(int_queue * Q);

#endif
