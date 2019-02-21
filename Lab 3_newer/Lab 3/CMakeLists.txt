cmake_minimum_required(VERSION 3.13)
project(Lab_3)
set(CMAKE_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lpthread -lrt")
include_directories(./COMMON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

file(GLOB planet-server
    "SERVER/*.c"
    "SERVER/*.h"
    "COMMON/*.c"
    "COMMON/*.h"
        )
add_executable(RUN_SERVER ${planet-server})
target_link_libraries(RUN_SERVER rt)
target_include_directories(RUN_SERVER PUBLIC ${GTK_INCLUDE_DIRS})
target_link_libraries(RUN_SERVER ${GTK_LIBRARIES})

file(GLOB planet-client
        "CLIENT/*.c"
        "CLIENT/*.h"
        "COMMON/*.c"
        "COMMON/*.h"
        )
add_executable(RUN_CLIENT ${planet-client})
target_link_libraries(RUN_CLIENT rt)
target_include_directories(RUN_CLIENT PUBLIC ${GTK_INCLUDE_DIRS})


#add_executable(Lab_3
 #       client.c
  #      wrapper.c
   #     wrapper.h
    #    server.c)
