#include "position.h"
#include "map.h"
#include "string_queue.h"
#include "load_map.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>

int str_to_size_t(const char * str, size_t * output)
{
	if (*str == '-')
		return 0;

	errno = 0;
	char * endptr;
	uintmax_t result = strtoumax(str, &endptr, 10);

	if (*str == '\0' || *endptr != '\0')
		return 0;

	if ((result == 0 || result == UINTMAX_MAX) && errno != 0)
		return 0;

	*output = (size_t) result;
	return 1;
}

int main(int argc, char * argv [])
{
	if (argc < 6)
	{
		printf(
			"Usage: \n"
			"%s [FILE] [SOURCE X] [SOURCE_Y] [DESTINATION_X] [DESTINATION_Y]\n", 
			argv[0]
		);
		return 1;
	}

	size_t width = 0;
	string_queue * map_q = load_map(argv[1], &width);
	if (!map_q) 
	{
		switch (file_load_error)
		{
			case file_load_error_file:
				printf("File could not be read!\n");
				break;
			case file_load_error_memory:
				printf("Memory allocation failed!\n");
				break;
			default:
				printf("Unknown error!\n");
		}
		return 2;
	}

	map_grid * map_g = create_map_grid(map_q, width);
	free_string_queue(map_q);
	if (!map_g)
	{
		printf("Map creation failed!\n");
		return 3;
	}

	position source = {0, 0};
	position dest = {0, 0};

	if (!str_to_size_t(argv[2], &(source.x)))
	{
		printf("Argument \"%s\" not correctly formated!\n", argv[2]);
		free_map_grid(map_g);
		return 4;
	}
	if (!str_to_size_t(argv[3], &(source.y)))
	{
		printf("Argument \"%s\" not correctly formated!\n", argv[3]);
		free_map_grid(map_g);
		return 4;
	}

	if (!str_to_size_t(argv[4], &(dest.x)))
	{
		printf("Argument \"%s\" not correctly formated!\n", argv[4]);
		free_map_grid(map_g);
		return 4;
	}
	if (!str_to_size_t(argv[5], &(dest.y)))
	{
		printf("Argument \"%s\" not correctly formated!\n", argv[5]);
		free_map_grid(map_g);
		return 4;
	}
#ifdef PROJECT_PRINT_AS_MAP
	if (!find_and_print_path_map_as_map(map_g, source, dest, 1))
	{
		printf("Path search function failed! (path might exist)\n");
		free_map_grid(map_g);
		return 5;
	}
#else
	if (!find_and_print_path(map_g, source, dest))
	{
		printf("Path search function failed! (path might exist)\n");
		free_map_grid(map_g);
		return 5;
	}
	else
	{
		putchar((int) '\n');
	}
#endif

	free_map_grid(map_g);
	return 0;
}
