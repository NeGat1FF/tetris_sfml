cmake_minimum_required(VERSION 3.22)
project(SFML_TEST)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/SFML")

# set include folder
include_directories(include)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS audio graphics system REQUIRED)

add_executable(Tetris src/main.cpp src/block.cpp include/block.h icon.rc)
target_link_libraries(Tetris sfml-audio sfml-graphics sfml-system sfml-window)
