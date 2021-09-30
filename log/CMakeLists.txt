cmake_minimum_required(VERSION 3.15)

project(${LOG_LIB_NAME} CXX)

set(LOG_LIB_SRCS
        "src/loggers/BaseLogger.cpp"
        "src/loggers/FileLogger.cpp"
        "src/loggers/StdoutLogger.cpp"
        "src/loggers/StderrLogger.cpp"

        "src/loggers/Logger.cpp"
        "src/loggers/ThreadedLogger.cpp"

        "src/format/BaseFormatter.cpp"
        "src/format/LogFormatter.cpp")

find_package(Threads)

add_library(${LOG_LIB_NAME} STATIC ${LOG_LIB_SRCS})

target_link_libraries(${LOG_LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(${LOG_LIB_NAME} PUBLIC include)
