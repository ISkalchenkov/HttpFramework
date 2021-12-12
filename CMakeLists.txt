cmake_minimum_required(VERSION 3.15)

project(HttpFramework)

set(CMAKE_CXX_STANDARD 20)

set(HTTP_LIB_NAME "http")
add_subdirectory(http)

set(LOG_LIB_NAME "log")
add_subdirectory(log)

add_subdirectory(examples)
