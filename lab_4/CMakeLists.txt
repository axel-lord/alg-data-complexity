cmake_minimum_required(VERSION 3.5)
project(Lab4)

set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -g -O0")

add_executable(ex_1_test ex_1_test.c graph.c merge_sort_uint.c)
target_link_libraries(ex_1_test m)

add_executable(ex_2_test ex_2_test.c graph.c depth_first_search.c)
target_link_libraries(ex_2_test m)
