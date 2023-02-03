cmake_minimum_required(VERSION 3.16)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_C_FLAGS: "-Wall -Wextra -02")

project(fs VERSION 1.0 LANGUAGES C)

add_executable(fs app/fs.c app/operations.c)

target_link_libraries(fs -lbsd)
