cmake_minimum_required(VERSION 2.8.9)

#
# Client exec
#
include_directories(include)
file(GLOB LIB_SOURCES "src/*.c")
project(client)
add_executable(${CMAKE_PROJECT_NAME} "client.c")

target_link_libraries(${CMAKE_PROJECT_NAME} progbase jansson ssl)
