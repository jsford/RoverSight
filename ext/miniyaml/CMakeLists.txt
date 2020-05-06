cmake_minimum_required(VERSION 3.1)

add_library(miniyaml yaml.cpp)
target_include_directories(miniyaml PUBLIC .)
