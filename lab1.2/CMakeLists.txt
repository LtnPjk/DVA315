cmake_minimum_required(VERSION 3.13)
project(lab1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_c_FLAGS} -pthread -lpthread -lrt")

include_directories(.)

add_executable(lab1
        wrapper.c
        wrapper.h
        intro2.c)

target_link_libraries(lab1 rt)