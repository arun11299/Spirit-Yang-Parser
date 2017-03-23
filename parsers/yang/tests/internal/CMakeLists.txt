
cmake_minimum_required(VERSION 2.8)

project(parser_tests)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})


include_directories("${PROJECT_SOURCE_DIR}")

add_executable(compile_test ${PROJECT_SOURCE_DIR}/compile_test.cc)
