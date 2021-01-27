#include <stdio.h>
#include <string.h>
#include "linked_list_int.h"

#define STRING_LENGTH 128

typedef struct min_max_s
{
	Node * min;
	Node * max;
} min_max;

typedef struct successor_predecessor_s
{
	Node * successor;
	Node * predecessor;
} successor_predecessor;

char * node_to_int_or_null(Node * n)
{
	static char null_str [] = "NULL";
	static char return_str [STRING_LENGTH] = {'\0'};
	if (n == NULL) {
		return null_str;
	}
	snprintf(return_str, STRING_LENGTH, "%d", n->data); //NOLINT
	return return_str;
}

/* Returns 0 on failure else 1*/
int arr_to_list(List * L, int * arr, int size)
{
	for (int i = 1; i <= size; ++i)
	{
		Node * n = createNode(*(arr+size-i));
		if (n == NULL)
			return 0;
		insert(L, n);
	}
	return 1;
}

/* Test Implementation of linked list
 * Returns 1 if memory allocation fails */
int main()
{
	List * L1;
	List * L2;

	int arr_L1 [] = {3, 1, 5, 10, 8, 7};
	int arr_L2 [] = {5, 2, 9, 6, 1, 2};
	int L1_l = sizeof(arr_L1)/sizeof(arr_L1[0]);
	int L2_l = sizeof(arr_L2)/sizeof(arr_L1[0]);

	L1 = createList();
	if (L1 == NULL)
		return 1;
	L2 = createList();
	if (L2 == NULL)
	{
		freeList(L1);
		return 1;
	}

	if (arr_to_list(L1, arr_L1, L1_l) && 
		arr_to_list(L2, arr_L2, L2_l))
	{
		min_max mm_L1 = {
			minimum(L1),
			maximum(L1)
		};
		min_max mm_L2 = {
			minimum(L2),
			maximum(L2)
		};

		printf("What are the minimum and maximum of L1?\n");
		printf("Min: %s, ", node_to_int_or_null(mm_L1.min));
		printf("Max: %s\n\n", node_to_int_or_null(mm_L1.max)); 

		printf("What are the minimum and maximum of L2?\n");
		printf("Min: %s, ", node_to_int_or_null(mm_L2.min));
		printf("Max: %s\n\n", node_to_int_or_null(mm_L2.max));

		Node * n5 = search(L1, 5);
		Node * n9 = search(L2, 9);
		if (n5 != NULL)
		{
			successor_predecessor s_p = {
				successor(L1, n5),
				predecessor(L1, n5)
			};
			printf("What is the successor and predecessor of the node with key 5 in L1?\n");
			printf("Successor: %s, ", node_to_int_or_null(s_p.successor));
			printf("Predecessor: %s\n\n", node_to_int_or_null(s_p.predecessor));
		} 
		else 
		{
			printf("Key 5 does not exist in L1\n\n");
		}
		if (n9 != NULL)
		{
			successor_predecessor s_p = {
				successor(L2, n9),
				predecessor(L2, n9)
			};
			printf("What is the successor and predecessor of the node with key 9 in L2?\n");
			printf("Successor: %s, ", node_to_int_or_null(s_p.successor));
			printf("Predecessor: %s\n\n", node_to_int_or_null(s_p.predecessor));
		}
		else
		{
			printf("Key 9 does not exist in L2\n\n");
		}
		if (mm_L1.max != NULL)
		{
			printf("What is the key of the predecessor in L2 of the maximum of L1?\n");
			printf("Key: %s\n\n", node_to_int_or_null(predecessor(L2, mm_L1.max)));
		}
		else
			printf("L1 does not hava a maximum!\n\n");
		if (mm_L2.max != NULL)
		{
			printf("What is the key of the predecessor in L1 of the maximum of L2?\n");
			printf("Key: %s\n\n", node_to_int_or_null(predecessor(L1, mm_L2.max)));
		}
		else
			printf("L2 does not have a maximum?\n");
	}
	
	freeList(L1);
	freeList(L2);
	return 0;
}
