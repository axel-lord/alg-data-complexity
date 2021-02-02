cmake_minimum_required(VERSION 3.5)
project(Lab1)

set(CMAKE_C_FLAGS "-std=c11 -Wall -g -O0")

add_executable(ex_4 ex_4_test.c merge_sort.c quick_sort.c)

