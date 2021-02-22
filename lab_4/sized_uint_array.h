#ifndef SIZED_ARRAY_HEADER
#define SIZED_ARRAY_HEADER

#define FREE_SIZED_ARRAY(a) {free(a->array); free(a);}

typedef struct sized_int_array_s
{
	unsigned int size;
	unsigned int * array;
} sized_uint_array;

#endif
