cmake_minimum_required(VERSION 3.9)
project(json C)

set(CMAKE_C_STANDARD 11)

add_executable(json main.c config.h exception.c exception.h json.c json.h test.c test.h json_types.c json_types.h helpers.c helpers.h stack.c stack.h input.c input.h)