#include "map_bfs.h"
#include "map.h"
#include "position.h"
#include <math.h>

typedef enum color_e {black, gray, white} color;

int map_grid_bfs(
	map_grid * map, 
	position source, 
	size_t height, 
	size_t width, 
	position path_prev[height][width],
	double distance[height][width]
) {
	if (!map_grid_position_valid(map, source))
		return 0;

	color (*position_color)[width];
	position_color = malloc(height * sizeof(*position_color));
	if (!position_color) return 0;

	position_queue * queue = create_position_queue();
	if (!queue)
	{
		free(position_color);
		return 0;
	}

	for (size_t c = 0; c < width; ++c)
		for (size_t r = 0; r < height; ++r)
		{
			path_prev[r][c].x = c;
			path_prev[r][c].y = r;
			position_color[r][c] = white;
			distance[r][c] = INFINITY;
		}
	distance[source.y][source.x] = 0;

	if (!position_enqueue(queue, source))
	{
		free(position_color);
		free_position_queue(queue);
		return 0;
	}

	position current_pos;
	while (position_dequeue(queue, &current_pos))
	{
		position_queue * neighbors = map_grid_get_neighbors(map, current_pos);
		if (!neighbors)
		{
			free(position_color);
			free_position_queue(queue);
			return 0;
		}

		position n;
		while (position_dequeue(neighbors, &n))
			if (position_color[n.y][n.x] == white)
			{
				position_color[n.y][n.x] = gray;
				distance[n.y][n.x] = distance[current_pos.y][current_pos.x] + 1;
				path_prev[n.y][n.x].x = current_pos.x;
				path_prev[n.y][n.x].y = current_pos.y;

				if (!position_enqueue(queue, n))
				{
					free(position_color);
					free_position_queue(queue);
					free_position_queue(neighbors);
					return 0;
				}
			}

		free_position_queue(neighbors);
		position_color[current_pos.y][current_pos.x] = black;
	}
		
	free_position_queue(queue);
	free(position_color);
	return 1;
}
