#include "load_map.h"
#include "map.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum file_load_error_e file_load_error = file_load_error_file;

typedef struct dyn_str_s
{
	size_t length;
	size_t array_size;
	char * str;
} dyn_str;

static int dyn_str_add_char(dyn_str * dest, char c)
{
	++dest->length;
	if (dest->length >= dest->array_size)
	{
		size_t new_size = dest->array_size * 2;
		char * new_str = realloc(dest->str, new_size);
		if (!new_str) return 0;
		dest->str = new_str;
		dest->array_size = new_size;
	}
	dest->str[dest->length-1] = c;
	dest->str[dest->length] = '\0';
	return 1;
}

static int dyn_str_init(dyn_str * str)
{
	str->array_size = 64;
	str->length = 0;
	str->str = malloc(str->array_size);
	if (!str->str) return 0;
	str->str[0] = '\0';
	return 1;
}

typedef enum line_read_r_e {LR_FAILURE, LR_EOF, LR_EOL} line_read_r;

static line_read_r line_to_dyn_str(FILE * file_ptr, dyn_str * dest)
{
	for(;;)
	{
		int r_int = fgetc(file_ptr);
		if (r_int == EOF)
			return LR_EOF;

		char r_char = (char) r_int;
		if (r_char == '\r')
			continue;
		if (r_char == '\n')
			return LR_EOL;
		
		if (r_char == '0') 
			r_char = PASS_THROUGH_CHAR;
		else if (r_char != PASS_THROUGH_CHAR) 
			r_char = WALL_CHAR;

		if (!dyn_str_add_char(dest, r_char))
			return LR_FAILURE;
	}

	return LR_EOF; // should never be reached
}

string_queue * load_map(const char * filename, size_t * width)
{
	FILE * map_file = fopen(filename, "r");
	if (!map_file) 
	{
		file_load_error = file_load_error_file;
		return NULL;
	}

	string_queue * q = create_string_queue();
	if (!q) 
	{
		fclose(map_file);
		file_load_error = file_load_error_memory;
		return NULL;
	}

	dyn_str line_1;
	if (!dyn_str_init(&line_1))
	{
		fclose(map_file);
		free_string_queue(q);
		file_load_error = file_load_error_memory;
		return NULL;
	}

	line_read_r result = line_to_dyn_str(map_file, &line_1);
	if (result == LR_FAILURE)
	{
		fclose(map_file);
		free_string_queue(q);
		free(line_1.str);
		file_load_error = file_load_error_memory;
		return NULL;
	}
	size_t w = line_1.length;

	if (w <= 0)
	{
		fclose(map_file);
		free_string_queue(q);
		free(line_1.str);
		file_load_error = file_load_error_impossible;
		return NULL;
	}

	if (!string_queue_enqueue(q, line_1.str, line_1.length))
	{
		fclose(map_file);
		free_string_queue(q);
		free(line_1.str);
		file_load_error = file_load_error_memory;
		return NULL;
	}
	free(line_1.str); // never needed again

	dyn_str line_i;
	if(!dyn_str_init(&line_i))
	{
		fclose(map_file);
		free_string_queue(q);
		file_load_error = file_load_error_memory;
		return NULL;
	}
	for (;;)
	{
		result = line_to_dyn_str(map_file, &line_i);
		if (result == LR_FAILURE)
		{
			fclose(map_file);
			free_string_queue(q);
			free(line_i.str);
			file_load_error = file_load_error_memory;
			return NULL;
		}
		if (result == LR_EOF) // if eof reached
			break;

		while (line_i.length < w)
			if (!dyn_str_add_char(&line_i, ' '))
			{
				fclose(map_file);
				free_string_queue(q);
				free(line_i.str);
				file_load_error = file_load_error_memory;
				return NULL;
			}

		if (!string_queue_enqueue(q, line_i.str, line_i.length))
		{
			fclose(map_file);
			free_string_queue(q);
			free(line_i.str);
			file_load_error = file_load_error_memory;
			return NULL;
		}

		line_i.length = 0;
		line_i.str[0] = '\0';
	}
	free(line_i.str);

	fclose(map_file);
	*width = w;
	return q;
}
