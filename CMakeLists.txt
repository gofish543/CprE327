cmake_minimum_required(VERSION 3.13)
project(CprE327)

set(CMAKE_CXX_STANDARD 14)

file(GLOB INCLUDES "include/*")
file(GLOB SOURCES "src/*")

add_executable(CprE327 ${INCLUDES} ${SOURCES})
