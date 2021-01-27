cmake_minimum_required(VERSION 3.5)
project(Lab1)

set(CMAKE_C_FLAGS "-std=c11 -Wall -g -O0")

add_executable(ex_2 ex_2_test.c linked_list_int.c)

add_executable(ex_5 ex_5_test.c queue_int_linked_list.c stack_int_linked_list.c linked_list_int.c)

