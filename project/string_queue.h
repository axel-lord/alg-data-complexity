#ifndef PROJECT_STRING_QUEUE_H
#define PROJECT_STRING_QUEUE_H

#include <string.h>

/* string queue type */
typedef struct string_queue_s string_queue;

/* create a string queue
 * Returns:
 * NULL, on failure
 * a pointer to a string_queue otherwise
 */
string_queue * create_string_queue();

/* free a sting queue and all strings contained in it */
void free_string_queue(string_queue * q);

/* copy given string and enqueue it
 * Parameters:
 * q, queue to enqueue to
 * source, string to copy
 * length, lenght of the string (not including terminator)
 * Returns:
 * 0, on failure
 * 1, otherwise
 */
int string_queue_enqueue(string_queue * q, const char * source, size_t length);

/* point given pointer to string and
 * dequeue it
 * Parameters:
 * q, queue to dequeue from
 * destination, pointer to pointer to point to string
 * Returns:
 * 0, on failure
 * length of the string otherwise
 */
size_t string_queue_dequeue(string_queue * q, char ** destination);

/* get the size of a string queue */
size_t string_queue_get_size(string_queue * q);

/* get if a string queue is empty, returns 1 if empty else 0 */
int string_queue_is_empty(string_queue * q);

/* print and empty a string queue */
void print_string_queue(string_queue * q);

#endif // PROJECT_STRING_QUEUE_H
