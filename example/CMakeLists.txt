cmake_minimum_required(VERSION 2.8)

set(project example)

project(example)

include_directories(header)

set(SOURCE_FILES
        database.hpp
        db.cpp
        )

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

target_link_libraries(../)


add_executable(${project} ${SOURCE_FILES})
target_link_libraries(${project} smart_executor ${CMAKE_THREAD_LIBS_INIT})
