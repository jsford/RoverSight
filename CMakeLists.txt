cmake_minimum_required(VERSION 3.1)

project(RoverSight CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(ext)

# Build the roversight library.
find_package(PopSift REQUIRED)
find_package(fmt REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
add_library(roversight
    src/roversight.cpp
    src/camera.cpp
    src/image.cpp
)
target_include_directories(roversight PUBLIC include/ PRIVATE src/)
target_link_libraries(roversight
    PUBLIC
    Eigen3::Eigen
    PRIVATE
    popsift
    fmt
    miniyaml
    stb
)

# Build the demo application.
add_executable(demo demo/main.cpp)
target_link_libraries(demo PRIVATE roversight stdc++fs)
target_compile_options(demo PRIVATE -O3)
