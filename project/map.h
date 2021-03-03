#ifndef PROJECT_MAP_H
#define PROJECT_MAP_H

#include "string_queue.h"
#include "position.h"

#define PASS_THROUGH_CHAR ' '
#define WALL_CHAR '#'
#define PATH_CHAR '+'
#define SOURCE_CHAR 'S'
#define DEST_CHAR 'D'

/* type to store map */
typedef struct map_grid_s map_grid;

/* Build a map from an array of strings
 * Parameters:
 * map_queue, queue to build from
 * width, map width
 * Returns:
 * NULL, if map creation fails
 * a map grid pointer, otherwise
 */
map_grid * create_map_grid(string_queue * map_queue, size_t width);

/* free a map_grid */
void free_map_grid(map_grid * map);

/* see if position is valid
 * Parameters:
 * map, map to check in
 * pos, position to check
 * Returns:
 * 1, if the position exists
 * 0, otherwise
 */
int map_grid_position_valid(const map_grid * map, position pos);

/* get valid neighboring positions in a map
 * Parameters:
 * map, map to check in
 * pos, position to check around
 * Returns:
 * NULL, on failure
 * a queue pointer with the neighboring positions otherwise
 */
position_queue * map_grid_get_neighbors(const map_grid * map, position pos);

/* get the width of a map */
size_t map_grid_get_width(map_grid * map);

/* get the height of a map */
size_t map_grid_get_height(map_grid * map);

/* print a map, and optionally a path on it
 * Parameters:
 * map, map to print
 * pos_s, position stack to use as give NULL if no path desired
 * use_color, wether to print colored text or not, set to 1 for color
 * Returns:
 * 0, if printing failed
 * 1, otherwise
 */
int print_map_grid(map_grid * map, position_stack * pos_s, int use_color);

/* print a path between two points on a map 
 * Parameters:
 * map, map to find path on
 * source, position to find from
 * dest, position to find
 * Returns:
 * 0, on failure
 * 1, on success (including if the path did not exist)
 */
int find_and_print_path(map_grid * map, position source, position dest);

/* print a path between two points on a map
 * print result as a map
 * Parameters:
 * map, map to find path on
 * source, position to find from
 * dest, position to find
 * use_color, if set to 1 color is used in output
 * Returns:
 * 0, on failure
 * 1, on success (including if the path did not exist)
 */
int find_and_print_path_map_as_map(map_grid * map, position source, position dest, int use_color);

#endif // PROJECT_MAP_H
