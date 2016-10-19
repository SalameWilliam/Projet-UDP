cmake_minimum_required(VERSION 3.6)
project(S01)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES packet_implem.c)
add_executable(S01 ${SOURCE_FILES})