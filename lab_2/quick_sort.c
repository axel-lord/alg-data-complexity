#include "sized_array.h"
#include "quick_sort.h"

int quicksort_array(sized_array * Arr)
{
	if (Arr->size <= 1)
		return 1;

	unsigned int iter = 0;
	int r_int = Arr->array[Arr->size-1];
	int swap;

	for (unsigned int i = 0; i < Arr->size-1; ++i)
	{
		if (Arr->array[i] < r_int)
		{
			swap = Arr->array[i];
			Arr->array[i] = Arr->array[iter];
			Arr->array[iter] = swap;
			++iter;
		}
	}
	Arr->array[Arr->size-1] = Arr->array[iter];
	Arr->array[iter] = r_int;
	
	sized_array arr_1 = 
	{
		iter, // size
		Arr->array // array
	};
	sized_array arr_2 =
	{
		Arr->size - iter - 1, // size
		Arr->array + iter + 1 // array
	};

	quicksort_array(&arr_1);
	quicksort_array(&arr_2);

	return 1;
}
