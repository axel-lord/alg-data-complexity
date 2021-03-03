#ifndef PROJECT_MAP_BFS_H
#define PROJECT_MAP_BFS_H

#include "map.h"
#include "position.h"
#include <stdlib.h>

/* Calculate shortest path
 * to each position and writes
 * to given matrix
 * Parameters:
 * map, map to calulate on
 * source, position to search from
 * height, height of map
 * width, width of map
 * path_prev, matrix to write result to
 * distance, matrix to write distance to
 * Returns:
 * 0, on failure
 * 1, otherwise
 */
int map_grid_bfs(
	map_grid * map, 
	position source, 
	size_t height, 
	size_t width, 
	position path_prev[height][width],
	double distance[height][width]
);

#endif // PROJECT_MAP_DFS_H
