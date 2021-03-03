cmake_minimum_required(VERSION 3.5)
project(project)

set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -g -O0")

add_executable(find_path_list find_path.c map.c string_queue.c position.c load_map.c map_bfs.c)

add_executable(find_path_map find_path.c map.c string_queue.c position.c load_map.c map_bfs.c)
target_compile_definitions(find_path_map PUBLIC -DPROJECT_PRINT_AS_MAP)
