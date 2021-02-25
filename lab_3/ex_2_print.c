#include "print_sorted.h"
#include "load_file.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv [])
{
	if (argc < 2)
	{
		printf("Usage: %s FILE...\n", argv[0]);
	}
	for (int i = 1; i < argc; ++i)
	{
		int * arr = load_file(argv[i]);
		print_sorted(*arr, arr+1);
		free(arr);
	}
	return 0;
}
