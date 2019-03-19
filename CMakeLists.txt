cmake_minimum_required(VERSION 3.13)
project(CprE327)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_LIBRARIES "-lncurses")

file(GLOB_RECURSE INCLUDE
        "include/*.h"
        "include/*.cpp")

file(GLOB_RECURSE SRC
        "src/*.h"
        "src/*.cpp")

add_executable(CprE327 ${INCLUDE} ${SRC})