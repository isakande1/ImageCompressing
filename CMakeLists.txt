cmake_minimum_required(VERSION 3.26)
project(compress C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -Wall -Wextra -Wpedantic -Werror -lm")

add_executable(compress main.c)
target_link_libraries(compress m )
