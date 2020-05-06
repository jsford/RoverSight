cmake_minimum_required(VERSION 3.1)

add_library(stb stb.cpp)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
