#include "print_sorted.h"
#include "binary_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

static int is_number(char * str, int length)
{
	for (int i = 0; i < length; ++i)
		if (!isdigit(str[i]))
			switch (str[i])
			{
				case '-':
				case '+':
					continue;
				default:
					return 0;
			}
	return 1;
}

int main(int argc, char * argv [])
{
	/*
	int arr [] = {5, 3, 2, 4, 1, 8, 7, 6, 9, 0};
	int arr_size = sizeof(arr) / sizeof(arr[0]);
	print_sorted(arr_size, arr);
	*/
	
	int arr [argc-1];
	int arr_size = argc-1;

	for (int i = 1; i < argc; ++i)
		if (is_number(argv[i], strlen(argv[i])))
			arr[i-1] = atoi(argv[i]);
		else
			--arr_size;

	print_sorted(arr_size, arr);

	return 0;
}
