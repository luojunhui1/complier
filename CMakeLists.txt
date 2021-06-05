cmake_minimum_required(VERSION 3.0.0)
project(complier VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_BUILD_TYPE DEBUG)

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB SOURCES "*.c")
add_executable(complier ${SOURCES})

