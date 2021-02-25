cmake_minimum_required(VERSION 3.5)
project(Lab3)

set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -g -O0")

add_executable(ex_1_test ex_1_test.c binary_tree.c)
add_executable(ex_1_alg ex_1_alg.c print_sorted.c binary_tree.c)

add_executable(ex_2_print ex_2_print.c print_sorted.c binary_tree.c load_file.c)
