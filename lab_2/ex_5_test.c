#include "merge_sort.h"
#include "quick_sort.h"
#include "load_file.h"
#include "sized_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define FILE_NAME_MAX_LENGTH 256
#define PATH_MAX_LENGTH 128
#define COUNT_MAX_LENGTH 32
#define PROBLEM_COUNT 25

#define FILENAME_TEMPLATE "%stest-%s-%d-problem"

#define COL_RESET "\033[0m"
#define COL_YELLOW "\033[1;93m"
#define COL_RED "\033[1;91m"
#define COL_GREEN "\033[92m"
#define COL_BOLD "\033[1m"

int (*sort_function)(sized_array*) = &quicksort_array;

int is_array_sorted(int * arr, unsigned int size)
{
	for (unsigned int i = 1; i < size; ++i)
		if (arr[i] < arr[i-1])
			return 0;
	return 1;
}

int sort_array(int * arr, unsigned int size, char * _)
{
	sized_array sort_arr = {
		size,
		arr
	};
	return (*sort_function)(&sort_arr);
}

int sort_array_verbose(int * arr, unsigned int size, char * name)
{
	printf( "\t" "testing: %s" "\n", name );
	if (!sort_array(arr, size, NULL)) {
		return 0;
	}

	if (!is_array_sorted(arr, size))
		printf( "\t\t" COL_RED "Problem sorting array!" COL_RESET "\n" );
	else
		printf( "\t\t" COL_GREEN "Array sorted!" COL_RESET "\n" );

	return 1;
}

void print_array(int * array, unsigned int size)
{
	if (array == NULL)
	{
		printf("NULL");
		return;
	}
	if (size == 0)
	{
		printf("Empty Array");
		return;
	}
	printf("%d", array[0]);
	for (int i = 1; i < size; ++i)
	{
		printf(", %d", array[i]);
	}
}

void free_problem_matrix(int * problem_matrix [PROBLEM_COUNT])
{
	for (int i = 0; i < PROBLEM_COUNT; ++i)
		free(problem_matrix[i]);
}

int build_problem_matrix(int * problem_matrix [PROBLEM_COUNT], char problem_names [PROBLEM_COUNT][FILE_NAME_MAX_LENGTH], char folder_path [PATH_MAX_LENGTH])
{
	char file_name [FILE_NAME_MAX_LENGTH]; // overkill but better safe than sorry
	file_name[0] = '\0';

	char arr_length [COUNT_MAX_LENGTH]; // overkill again
	arr_length[0] = '1';
	arr_length[1] = '\0'; // problem if COUNT_MAX_LENGTH is < 2

	int k = 0;
	for (int i = 1;k < PROBLEM_COUNT && i < COUNT_MAX_LENGTH-1; ++i) 
	{
		arr_length[i] = '0';
		arr_length[i+1] = '\0';

		for (int j = 1; j <= 5 && k < PROBLEM_COUNT; ++j)
		{
			snprintf(file_name, FILE_NAME_MAX_LENGTH, FILENAME_TEMPLATE, folder_path, arr_length, j); //NOLINT
			int * arr = load_file(file_name); // malloced memory

			// memory will not be manually freed
			// should load_file fail since it exits
			// on failure hopefully system frees memory
			// otherwise big problem

			if (arr == NULL) // arr at this point cannot be NULL since load_file does not return NULL
			{
				free_problem_matrix(problem_matrix); // all malloced freed
				return 0;
			}

			problem_matrix[k] = arr;
			strncpy(problem_names[k], file_name, FILE_NAME_MAX_LENGTH); //NOLINT
			++k;
		}
	}
	return 1;
}


int test_alg(int (*fun_ptr)(int*, unsigned int, char*), clock_t * tick_amount, char folder_path [PATH_MAX_LENGTH])
{
	char problem_names [PROBLEM_COUNT][FILE_NAME_MAX_LENGTH]; // all names on stack
	int * problem_matrix [PROBLEM_COUNT] = {NULL};

	if (!build_problem_matrix(problem_matrix, problem_names, folder_path)) // mallocs memory
	{
		printf("Problem building array matrix!\n");
		return 0;
	}
	
	clock_t tick_0 = clock();

	for (int i = 0; i < PROBLEM_COUNT; ++i)
	{
		if (problem_matrix[i] == NULL)
		{
			printf("NULL ptr in array matrix!\n");
			free_problem_matrix(problem_matrix); // frees memory
			return 0;
		}

		if (!(*fun_ptr)(problem_matrix[i]+1, problem_matrix[i][0], problem_names[i])) // sort array
		{
			printf("Array sort failed for: %s\n", problem_names[i]);
			free_problem_matrix(problem_matrix); // frees memory
			return 0;
		}
	}

	clock_t time_used = clock() - tick_0;
	if (tick_amount != NULL)
		*tick_amount = time_used;

	free_problem_matrix(problem_matrix); // frees memory

	return 1;
}


int test_on_files(char folder_path [PATH_MAX_LENGTH])
{	
	char * function_name = COL_YELLOW "%s" COL_RESET ":" "\n";

	// Test functions
	printf( COL_BOLD "Test functions on all arrays:" COL_RESET "\n" );
	printf( function_name, "Mergesort" );
	sort_function = &mergesort_array;
	test_alg( &sort_array_verbose, NULL, folder_path);

	printf( function_name, "Quicksort" );
	sort_function = &quicksort_array;
	test_alg( &sort_array_verbose, NULL, folder_path);
	
	// Time functions
	clock_t ticks_used_ms = 0;
	clock_t ticks_used_qs = 0;

	printf( "\n" COL_BOLD "Ticks used to sort all arrays:" COL_RESET "\n" );
	printf(function_name, "Mergesort");
	sort_function = &mergesort_array;
	if (!test_alg(&sort_array, &ticks_used_ms, folder_path))
		return 0;
	printf("\t" "Ticks Passed: %lf, In seconds: %lf" "\n", (double)ticks_used_ms, (double)ticks_used_ms / CLOCKS_PER_SEC);

	printf(function_name, "Quicksort");
	sort_function = &quicksort_array;
	if (!test_alg(&sort_array, &ticks_used_qs, folder_path))
		return 0;
	printf("\t" "Ticks Passed: %lf, In seconds: %lf" "\n", (double)ticks_used_qs, (double)ticks_used_qs / CLOCKS_PER_SEC);
	
	printf( "\n" COL_BOLD "Comparison:" COL_RESET "\n" );
	printf(
		COL_YELLOW "Mergesort" COL_RESET " is %lf times faster than quicksort!" "\n",
		(double)ticks_used_qs / ticks_used_ms
	);
	printf(
		COL_YELLOW "Quicksort" COL_RESET " is %lf times faster than mergesort!" "\n",
		(double)ticks_used_ms / ticks_used_qs
	);

	return 1;
}

int test_sort_on_array(const char * filename)
{
	printf( COL_BOLD "Sorting:" COL_RESET " %s" "\n", filename);
	int * arr = load_file(filename); // mallocced memory
	if (arr == NULL) {
		printf("File load failed!" "\n");
		return 0;
	}

	int size = arr[0];
	int arr_ms [size];
	int arr_qs [size];

	for (int i = 1; i <= arr[0]; i++) {
		arr_ms[i-1] = arr[i];
		arr_qs[i-1] = arr[i];
	}

	free(arr); // memory freed when no longer needed
	
	clock_t tick_0;
	sort_function = &mergesort_array;
	tick_0 = clock();
	int result = sort_array(arr_ms, size, NULL);
	clock_t count_1 = clock() - tick_0;

	sort_function = &quicksort_array;
	tick_0 = clock();
	result *= sort_array(arr_qs, size, NULL);
	clock_t count_2 = clock() - tick_0;
	
	if (!result)
		return 0;
		
	printf( COL_YELLOW "Mergesort" COL_RESET ":" "\n\t" COL_BOLD );
	if (size < 999)
	{
		print_array(arr_ms, size);
		printf(COL_RESET "\n");
	}
	printf(COL_BOLD "Time:" COL_RESET " %lf ticks, %lf seconds" "\n", (double) count_1, (double) count_1 / CLOCKS_PER_SEC);

	printf( COL_YELLOW "Quicksort" COL_RESET ":" "\n\t" COL_BOLD );
	if (size < 999)
	{
		print_array(arr_qs, size);
		printf(COL_RESET "\n");
	}
	printf(COL_BOLD "Time:" COL_RESET " %lf ticks, %lf seconds" "\n", (double) count_2, (double) count_2 / CLOCKS_PER_SEC);

	return 1;
}

int main(int argc, char * argv [])
{
	if (argc <= 1)
		return 1;

	char folder_path [PATH_MAX_LENGTH];
	unsigned long folder_path_length = strlen(argv[1]);

	int isFile = 1;
	if (argv[1][folder_path_length-1] == '/')
		isFile = 0;

	if (!isFile)
	{
		if (folder_path_length > PATH_MAX_LENGTH - 2) {
			printf("Path to folder can be at most %d characters long!\n", PATH_MAX_LENGTH - 2);
			return 1;
		}

		strncpy(folder_path, argv[1], PATH_MAX_LENGTH); //NOLINT
		folder_path[PATH_MAX_LENGTH-1] = '\0'; // probably not needed

		test_on_files(folder_path);
	}

	for (int i = 2-isFile; i < argc; ++i) {
		printf("%s", i == 2-isFile ? "" : "\n");
		test_sort_on_array(argv[i]);
	}

	return 0;
}
