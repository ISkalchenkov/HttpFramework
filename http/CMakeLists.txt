cmake_minimum_required(VERSION 3.15)

project(${HTTP_LIB_NAME} CXX)

set(HTTP_LIB_SRCS
        "src/coroutine/coroutine.cpp")

find_package(Threads)

add_library(${HTTP_LIB_NAME} STATIC ${HTTP_LIB_SRCS})

target_link_libraries(${HTTP_LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(${HTTP_LIB_NAME} PUBLIC include)
