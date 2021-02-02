#include "sized_array.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include <stdio.h>
#include <string.h>

#define EMPTY_LINE printf("\n")

void print_sized_arr(sized_array * arr)
{
	if (arr == NULL)
	{
		printf("NULL");
		return;
	}
	if (arr->size == 0)
	{
		printf("Empty Array");
	}
	printf("%d", arr->array[0]);
	for (int i = 1; i < arr->size; ++i)
	{
		printf(", %d", arr->array[i]);
	}
}

void reset_buffer(int * source, sized_array * buffer)
{
	memcpy(buffer->array, source, buffer->size * sizeof(int)); //NOLINT
}

static inline void reset_buffers(int * s_1, int * s_2, int * s_3, sized_array * b_1, sized_array * b_2, sized_array * b_3)
{
	reset_buffer(s_1, b_1);
	reset_buffer(s_2, b_2);
	reset_buffer(s_3, b_3);
}

static inline void print_arrays(
	sized_array * a_1, sized_array * a_2, sized_array * a_3, 
	const char * n_1,  const char * n_2,  const char * n_3
)
{
	printf(  "%s: ", n_1); print_sized_arr(a_1);
	printf("\n%s: ", n_2); print_sized_arr(a_2);
	printf("\n%s: ", n_3); print_sized_arr(a_3);
}


int main()
{
	int arr_l1 [] = {3, 1, 5, 10, 8, 7};     int l1_size    = sizeof(arr_l1) / sizeof(int);
	int arr_l2 [] = {5, 2, 9, 6, 1, 2};      int l2_size    = sizeof(arr_l2) / sizeof(int);
	int p_num [] = {2, 0, 0, 0, 0, 3, 0, 4}; int p_num_size = sizeof(p_num)  / sizeof(int);

	int l1_buffer [l1_size];
	int l2_buffer [l2_size];
	int p_num_buffer [p_num_size];

	sized_array l1_s_arr = {l1_size, l1_buffer};
	sized_array l2_s_arr = {l2_size, l2_buffer};
	sized_array p_num_s_arr ={p_num_size, p_num_buffer};

#define RESET reset_buffers(arr_l1, arr_l2, p_num, &l1_s_arr, &l2_s_arr, &p_num_s_arr)
#define PRINT print_arrays(&l1_s_arr, &l2_s_arr, &p_num_s_arr, "List1", "List2", "Personnummer")
	
	RESET;
	PRINT;
	EMPTY_LINE;

	quicksort_array(&l1_s_arr);
	quicksort_array(&l2_s_arr);
	quicksort_array(&p_num_s_arr);

	PRINT;
	EMPTY_LINE;
	RESET;

	mergesort_array(&l1_s_arr);
	mergesort_array(&l2_s_arr);
	mergesort_array(&p_num_s_arr);

	PRINT;
	EMPTY_LINE;

	return 0;
}
