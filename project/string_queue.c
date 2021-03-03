#include "string_queue.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct string_node_s
{
	char * string;
	size_t length;
	struct string_node_s * next;
} string_node;

struct string_queue_s
{
	size_t size;
	string_node * head;
	string_node * tail;
};

string_queue * create_string_queue()
{
	string_queue * q = malloc(sizeof(string_queue));
	if (!q) return NULL;
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;
}

void free_string_queue(string_queue * q)
{
	string_node * i = q->head;
	while (i)
	{
		string_node * d = i;
		i = i->next;
		free(d->string);
		free(d);
	}
	free(q);
}


int string_queue_enqueue(string_queue * q, const char * source, size_t length)
{
	string_node * n = malloc(sizeof(string_node));
	if (!n) return 0;
	char * str = calloc(length+1, sizeof(char));
	if (!str)
	{
		free(n);
		return 0;
	}
	strncpy(str, source, length+1); // NOLINT
	n->string = str;
	n->length = length;
	
	n->next = NULL;

	if (q->tail)
		q->tail->next = n;
	else
		q->head = n;
	q->tail = n;
	

	++q->size;
	return 1;
}


size_t string_queue_dequeue(string_queue * q, char ** destination)
{
	if (!q->head)
		return 0;

	string_node * d = q->head;
	size_t l = d->length;
	*destination = d->string;
	q->head = d->next;

	if (!q->head)
		q->tail = NULL;

	free(d);
	--q->size;
	return l;
}

size_t string_queue_get_size(string_queue * q)
{
	return q->size;
}

int string_queue_is_empty(string_queue * q)
{
	return q->head == NULL;
}

void print_string_queue(string_queue * q)
{
	char * str;
	while (string_queue_dequeue(q, &str))
		printf("%s\n", str);
}
