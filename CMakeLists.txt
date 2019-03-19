cmake_minimum_required(VERSION 3.13)
project(CprE327)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES "src/*.cpp")
file(GLOB TERRAINS "src/terrains/*.cpp")

add_executable(CprE327 ${SOURCES} ${TERRAINS} "./include/forward_declarations.h" src/vendor/queue.cpp src/vendor/queue.h)
