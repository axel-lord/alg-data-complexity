#ifndef PROJECT_LOAD_MAP_H
#define PROJECT_LOAD_MAP_H

#include "string_queue.h"
#include <string.h>

enum file_load_error_e {file_load_error_memory, file_load_error_file, file_load_error_impossible};

/* load a map file into a string queue
 * Parameters:
 * filename, path to file to load
 * width, pointer to variable to write map width to
 * Returns:
 * NULL, on failure, sets file_load_error to signify what went wrong
 * a string_queue pointer otherwise
 */
string_queue * load_map(const char * filename, size_t * width);

extern enum file_load_error_e file_load_error;

#endif // PROJECT_LOAD_MAP_H
