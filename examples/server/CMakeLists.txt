cmake_minimum_required(VERSION 3.15)

set(EX_SERVER_SRCS
        "main.cpp"
        "HttpServer.cpp")

add_executable(${EX_SERVER_NAME} ${EX_SERVER_SRCS})

target_link_libraries(${EX_SERVER_NAME} ${HTTP_LIB_NAME})

target_link_libraries(${EX_SERVER_NAME} ${LOG_LIB_NAME})

