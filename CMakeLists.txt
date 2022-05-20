cmake_minimum_required(VERSION 3.22)
project(SFML_TEST)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR $"C:/SFML")
find_package(SFML COMPONENTS audio graphics network system REQUIRED)

add_executable(Tetris WIN32 main.cpp block.cpp block.h icon.rc)
target_link_libraries(Tetris sfml-audio sfml-graphics sfml-network sfml-system sfml-window)
