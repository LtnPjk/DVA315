cmake_minimum_required(VERSION 3.10.2)
project(Lab_3 C)
set(CMAKE_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lpthread -lrt")
include_directories(.)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

file(GLOB Lab3
    "server.c"
    "wrapper.h"
    "wrapper.c"
        )
add_executable(RUN_SERVER ${Lab3})
target_link_libraries(RUN_SERVER rt)
target_include_directories(RUN_SERVER PUBLIC ${GTK_INCLUDE_DIRS})
target_link_libraries(RUN_SERVER ${GTK_LIBRARIES})

file(GLOB Lab3_client
    "client.c"
    "wrapper.h"
    "wrapper.c"
    )
add_executable(RUN_CLIENT ${Lab3_client})
target_link_libraries(RUN_CLIENT rt)
target_include_directories(RUN_CLIENT PUBLIC ${GTK_INCLUDE_DIRS})
