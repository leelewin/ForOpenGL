cmake_minimum_required(VERSION 3.11)
project(render+)
add_compile_options(-Wall -Wextra -g) #-Werror
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -std=c++17 -pthread")
find_package(glfw3 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)


