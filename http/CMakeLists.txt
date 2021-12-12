cmake_minimum_required(VERSION 3.15)

project(${HTTP_LIB_NAME})

set(HTTP_LIB_SRCS
        "src/coroutine/coroutine.cpp"
        "src/utils/file_descriptor.cpp"
        "src/utils/signal.cpp"
        "src/exception.cpp"
        "src/server/server.cpp"
        "src/server/server_worker.cpp"
        "src/server/connection.cpp"
        "src/server/client_handler.cpp"
        "src/protocol/http_response.cpp"
        "src/protocol/http_request.cpp"
        )

find_package(Threads)

add_library(${HTTP_LIB_NAME} STATIC ${HTTP_LIB_SRCS})

target_link_libraries(${HTTP_LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(${HTTP_LIB_NAME} PUBLIC include)
