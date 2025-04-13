cmake_minimum_required(VERSION 3.10)
project(cpp09 VERSION 1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -Werror)

add_subdirectory(ex00)
add_subdirectory(ex01)
add_subdirectory(ex02)
