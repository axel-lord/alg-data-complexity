#include "merge_sort_uint.h"
#include "sized_uint_array.h"
#include <stdlib.h>

int mergesort_array(sized_uint_array * Arr)
{
	if (Arr->size <= 1) return 1;

	sized_uint_array arr_1;
	sized_uint_array arr_2;

	unsigned int mid = Arr->size / 2;
	arr_1.size = mid;
	arr_2.size = Arr->size - mid;

	arr_2.array = Arr->array + mid;
	unsigned int arr_1_arr [mid]; 
	//arr_1.array = calloc(mid, sizeof(int));
	arr_1.array = arr_1_arr;
	if (arr_1.array == NULL)
		return 0;

	for (unsigned int i = 0; i < mid; ++i)
	{
		arr_1.array[i] = Arr->array[i];
	}
	
	if (!(mergesort_array(&arr_1)) || !(mergesort_array(&arr_2)))
	{
		//free(arr_1.array);
		return 0;
	}

	unsigned int i = 0; 
	unsigned int j = 0; 
	unsigned int k = 0;

	while (i < arr_1.size && j < arr_2.size)
	{
		if (arr_1.array[i] < arr_2.array[j])
		{
			Arr->array[k] = arr_1.array[i];
			++k;
			++i;
			continue;
		}
		Arr->array[k] = arr_2.array[j];
		++k;
		++j;
	}
	for (; i < arr_1.size; ++i)
	{
		Arr->array[k] = arr_1.array[i];
		++k;
	}
	for (; j < arr_2.size; ++j)
	{
		Arr->array[k] = arr_2.array[j];
		++k;
	}

	//free(arr_1.array);
	return 1;
}
