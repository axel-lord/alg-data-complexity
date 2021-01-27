#include <stdlib.h>
#include <limits.h>
#include "linked_list_int.h"

Node * createNode(int key)
{
	Node * new_node = malloc(sizeof(Node));
	if (new_node != NULL) {
		new_node->next = NULL;
		new_node->prev = NULL;
		new_node->data = key;
	}
	return new_node;
}

List * createList()
{
	List * new_list = malloc(sizeof(List));
	if (new_list == NULL) return NULL;

	Node * sentinel_node = createNode(0);
	if (sentinel_node == NULL)
	{
		free(new_list);
		return NULL;
	}

	sentinel_node->next = sentinel_node;
	sentinel_node->prev = sentinel_node;

	new_list->NIL = sentinel_node;
	return new_list;
}

void freeList(List * L)
{
	while (L->NIL->next != L->NIL)
	{
		L->NIL->prev = L->NIL->next->next; // store next node to be deleted
		free(L->NIL->next);
		L->NIL->next = L->NIL->prev; // set stored node to new head
	}

	free(L->NIL);
	free(L);
}

int isEmpty(List * L)
{
	return L->NIL->next == L->NIL;
}

int insert(List * L, Node * N)
{
	if (N == NULL) return 0; // allows chaining insert(L, createNode(n))

	N->next = L->NIL->next;
	N->prev = L->NIL;
	L->NIL->next->prev = N;
	L->NIL->next = N;

	return 1;
}

Node * search(List * L, int k)
{
	Node * iter_node = L->NIL->next;
	while (iter_node != L->NIL && iter_node->data != k)
		iter_node = iter_node->next;
	if (iter_node == L->NIL)
		return NULL;
	return iter_node;
}

Node * delete(List * L, Node * N)
{
	if (N == L->NIL || N->prev == NULL || N->next == NULL)
		return NULL;

	N->prev->next = N->next;
	N->next->prev = N->prev;

	N->next = NULL;
	N->prev = NULL;

	return N;
}

Node * maximum(List * L)
{
	if (isEmpty(L))
		return NULL;

	Node * max_node = L->NIL->next;
	Node * iter_node = L->NIL->next->next;

	while (iter_node != L->NIL)
	{
		if (iter_node->data >= max_node->data)
			max_node = iter_node;
		iter_node = iter_node->next;
	}

	return max_node;
}

Node * minimum(List * L)
{
	if (isEmpty(L))
		return NULL;

	Node * min_node = L->NIL->next;
	Node * iter_node = L->NIL->next->next;

	while (iter_node != L->NIL)
	{
		if (iter_node->data <= min_node->data)
			min_node = iter_node;
		iter_node = iter_node->next;
	}

	return min_node;
}

Node * successor(List * L, Node * N)
{
	if (isEmpty(L)) return NULL;

	L->NIL->data = INT_MAX;
	Node * successor_node = L->NIL;
	Node * iter_node = L->NIL->next;

	while (iter_node != L->NIL)
	{
		if (iter_node->data > N->data && iter_node->data <= successor_node->data)
			successor_node = iter_node;

		iter_node = iter_node->next;
	}
	
	if (successor_node == L->NIL)
		return NULL;
	return successor_node;
}

Node * predecessor(List * L, Node * N)
{
	if (isEmpty(L)) return NULL;

	L->NIL->data = INT_MIN;
	Node * predecessor_node = L->NIL;
	Node * iter_node = L->NIL->next;

	while (iter_node != L->NIL)
	{
		if (iter_node->data < N->data && 
			iter_node->data >= predecessor_node->data)
		{
			predecessor_node = iter_node;
		}
		iter_node = iter_node->next;
	}
	
	if (predecessor_node == L->NIL)
		return NULL;
	return predecessor_node;
}

