#include "map.h"
#include "string_queue.h"
#include "position.h"
#include "map_bfs.h"
#include <stdio.h>
#include <math.h>

struct map_grid_s
{
	char ** grid;
	size_t width;
	size_t height;
};

map_grid * create_map_grid(string_queue * map_queue, size_t width)
{
	map_grid * map = malloc(sizeof(map_grid));
	if (!map) return NULL;

	map->height = string_queue_get_size(map_queue);
	map->width = width;

	map->grid = calloc(map->height, sizeof(char*));
	if (!map->grid)
	{
		free(map);
		return NULL;
	}

	for (size_t i = 0; i < map->height; ++i)
	{
		char * str = NULL;
		size_t w = string_queue_dequeue(map_queue, &str);

		if (w < map->width)
		{
			for (size_t j = 0; j < i; ++j)
				free(map->grid[j]);
			free(map->grid);
			free(map);
			return NULL;
		}

		map->grid[i] = str;
	}

	return map;
}

void free_map_grid(map_grid * map)
{
	for (size_t i = 0; i < map->height; ++i)
		free(map->grid[i]);
	free(map->grid);
	free(map);
}

int map_grid_position_valid(const map_grid * map, position pos)
{
	if (pos.x >= map->width)
		return 0;
	if (pos.y >= map->height)
		return 0;
	return map->grid[pos.y][pos.x] == PASS_THROUGH_CHAR;
}

position_queue * map_grid_get_neighbors(const map_grid * map, position pos)
{
	position_queue * q = create_position_queue();
	if (!q) return NULL;

#define CHECK_POS(n, _X, _Y) \
	{\
		position n = {.x=_X, .y=_Y};\
		if (map_grid_position_valid(map, n))\
			if (!position_enqueue(q, n))\
			{\
				free_position_queue(q);\
				return NULL;\
			}\
	}

	if (pos.x > 0)
		CHECK_POS(left, pos.x-1, pos.y)

	if (pos.y > 0)
		CHECK_POS(up, pos.x, pos.y-1)
	
	CHECK_POS(right, pos.x+1, pos.y)

	CHECK_POS(down, pos.x, pos.y+1)

	return q;
#undef CHECK_POS
}

size_t map_grid_get_width(map_grid * map)
{
	return map->width;
}

size_t map_grid_get_height(map_grid * map)
{
	return map->height;
}

#define NEWLINE putchar((int) '\n')
#define COL_BORDER "\033[1;30;47m"
#define COL_WALL "\033[1;97;100m"
#define COL_SOURCE "\033[0;49;92m"
#define COL_DEST "\033[0;49;91m"
#define COL_PATH "\033[0;49;93m"
#define COL_RESET "\033[0m"

static void putchar_c(char c)
{
	putchar((int) c);
}

static void print_char_row(char c, size_t length)
{
	for (size_t i = 0; i < length; ++i) 
		putchar_c(c);
}

static int print_simple_noclr(map_grid * map)
{
	print_char_row(WALL_CHAR, map->width + 2);
	NEWLINE;

	for (size_t r = 0; r < map->height; ++r)
		printf("%c" "%s" "%c\n", WALL_CHAR, map->grid[r], WALL_CHAR);

	print_char_row(WALL_CHAR, map->width + 2);
	NEWLINE;
	return 1;
}

typedef enum print_state_e {s_wall, s_open, s_source, s_dest, s_path, s_border} print_state;

static int print_simple_clr(map_grid * map)
{
	printf(COL_BORDER);
	print_char_row(WALL_CHAR, map->width + 2);
	printf(COL_RESET);
	NEWLINE;

	for (size_t r = 0; r < map->height; ++r)
	{
		printf(COL_BORDER "%c", WALL_CHAR);
		
		// set starting state
		print_state state;
		if (map->grid[r][0] == WALL_CHAR)
		{
			printf(COL_WALL);
			state = s_wall;
		}
		else 
		{
			printf(COL_RESET);
			state = s_open;
		}
		
		for (size_t c = 0; c < map->width; ++c)
		{
			switch (state)
			{
				case s_wall:
					if (map->grid[r][c] == WALL_CHAR)
					{
						putchar_c(WALL_CHAR);
					}
					else
					{
						printf(COL_RESET "%c", PASS_THROUGH_CHAR);
						state = s_open;
					}
					break;
				case s_open:
					if (map->grid[r][c] == WALL_CHAR)
					{
						printf(COL_WALL "%c", WALL_CHAR);
						state = s_wall;
					}
					else
					{
						putchar_c(PASS_THROUGH_CHAR);
					}
					break;
				default:
					putchar_c('E');
			}
		}

		printf(COL_BORDER "%c" COL_RESET "\n", WALL_CHAR);
	}
	
	printf(COL_BORDER);
	print_char_row(WALL_CHAR, map->width + 2);
	printf(COL_RESET);
	NEWLINE;

	return 1;
}

static print_state char_to_state(char c)
{
	switch(c)
	{
		case WALL_CHAR:
			return s_wall;
		case SOURCE_CHAR:
			return s_source;
		case DEST_CHAR:
			return s_dest;
		case PATH_CHAR:
			return s_path;
		default:
			return s_open;
	}
}

static int print_path_noclr(size_t rows, size_t columns, char map_m[rows][columns])
{
	print_char_row(WALL_CHAR, columns + 1);
	NEWLINE;

	for (size_t l = 0; l < rows; ++l)
	{
		printf("%c" "%s" "%c" "\n", WALL_CHAR, map_m[l], WALL_CHAR); 
	}

	print_char_row(WALL_CHAR, columns + 1);
	NEWLINE;
	return 1;
}

static int print_path_clr(size_t rows, size_t columns, char map_m[rows][columns])
{
	printf(COL_BORDER);
	print_char_row(WALL_CHAR, columns + 1);
	printf(COL_RESET);
	NEWLINE;

	for (size_t l = 0; l < rows; ++l)
	{
		printf(COL_BORDER "%c", WALL_CHAR);
		print_state state = s_border;
		
		for (size_t col = 0; col < columns - 1; ++col)
		{
			print_state new_state = char_to_state(map_m[l][col]);
			if (state == new_state)
			{
				putchar_c(map_m[l][col]);
			}
			else
			{
				state = new_state;
				switch (state)
				{
					case s_wall:
						printf(COL_WALL);
						break;
					case s_border:
						printf(COL_BORDER);
						break;
					case s_open:
						printf(COL_RESET);
						break;
					case s_source:
						printf(COL_SOURCE);
						break;
					case s_path:
						printf(COL_PATH);
						break;
					case s_dest:
						printf(COL_DEST);
				}
				putchar_c(map_m[l][col]);
			}
		}

		printf(COL_BORDER "%c" COL_RESET "\n", WALL_CHAR);
	}

	printf(COL_BORDER);
	print_char_row(WALL_CHAR, columns + 1);
	printf(COL_RESET);
	NEWLINE;
	return 1;
}

int print_map_grid(map_grid * map, position_stack * pos_s, int use_color)
{
	int use_stack;
	if (pos_s != NULL) use_stack = !position_stack_is_empty(pos_s);
	else use_stack = 0;
	
	// no pos stack is used
	if (!use_stack)
	{
		if (!use_color)
			return print_simple_noclr(map);
		return print_simple_clr(map);
	}
	
	// pos stack is used
	char (*map_m)[map->width+1];
	map_m = malloc(sizeof(*map_m) * map->height);
	if (!map_m) return 0;

	for (size_t l = 0; l < map->height; ++l)
	{
		// memcpy is used since I see it more as a matrix of chars than an array of strings
		memcpy(map_m[l], map->grid[l], map->width); // NOLINT
		map_m[l][map->width] = '\0'; // makes sure all lines are terminated
	}

	position current_pos = {.x=0, .y=0};
	position_pop(pos_s, &current_pos);
	map_m[current_pos.y][current_pos.x] = SOURCE_CHAR;
	
	while (position_pop(pos_s, &current_pos))
		map_m[current_pos.y][current_pos.x] = PATH_CHAR;

	map_m[current_pos.y][current_pos.x] = DEST_CHAR;
	
	int result;
	if (use_color)
		result = print_path_clr(map->height, map->width+1, map_m);
	else
		result = print_path_noclr(map->height, map->width+1, map_m);

	free(map_m);
	return result;
}

#undef NEWLINE
#undef COL_BORDER
#undef COL_RESET
#undef COL_WALL

int find_and_print_path(map_grid * map, position source, position dest)
{
	
	if (!map_grid_position_valid(map, source) || !map_grid_position_valid(map, dest))
	{
		printf("NULL");
		return 1;
	}

	size_t w = map_grid_get_width(map);
	size_t h = map_grid_get_height(map);

	// create matrices
	double (*dist)[w];
	dist = malloc(sizeof(*dist) * h);
	if (!dist) 
	{
		return 0;
	}

	position (*prev)[w];
	prev = malloc(sizeof(*prev) * h);
	if (!prev)
	{
		free(dist);
		return 0;
	}
	
	// run bfs
	if (!map_grid_bfs(map, source, h, w, prev, dist))
	{
		free(dist);
		free(prev);
		return 0;
	}

	// chek if path possible
	if (isinf(dist[dest.y][dest.x]))
	{
		printf("NULL");
		free(dist);
		free(prev);
		return 1; // 1 is returned as this is a valid output
	}

	// build path
	position_stack * p_s = build_path_stack(source, dest, h, w, prev, dist);
	if (!p_s)
	{
		free(dist);
		free(prev);
		return 0;
	}
	
	print_position_stack(p_s);
	//int result = print_map_grid(map, p_s, 1);

	free(dist);
	free(prev);
	free_position_stack(p_s);

	return 1;
}

int find_and_print_path_map_as_map(map_grid * map, position source, position dest, int use_color)
{
	if (!map_grid_position_valid(map, source) || !map_grid_position_valid(map, dest))
	{
		printf("NULL\n");
		return 1;
	}

	size_t w = map_grid_get_width(map);
	size_t h = map_grid_get_height(map);

	// create matrices
	double (*dist)[w];
	dist = malloc(sizeof(*dist) * h);
	if (!dist) 
	{
		return 0;
	}

	position (*prev)[w];
	prev = malloc(sizeof(*prev) * h);
	if (!prev)
	{
		free(dist);
		return 0;
	}
	
	// run bfs
	if (!map_grid_bfs(map, source, h, w, prev, dist))
	{
		free(dist);
		free(prev);
		return 0;
	}

	// chek if path possible
	if (isinf(dist[dest.y][dest.x]))
	{
		printf("NULL\n");
		free(dist);
		free(prev);
		return 1; // 1 is returned as this is a valid output
	}

	// build path
	position_stack * p_s = build_path_stack(source, dest, h, w, prev, dist);
	if (!p_s)
	{
		free(dist);
		free(prev);
		return 0;
	}
	
	//print_position_stack(p_s);
	int result = print_map_grid(map, p_s, use_color);

	free(dist);
	free(prev);
	free_position_stack(p_s);

	return result;
}
