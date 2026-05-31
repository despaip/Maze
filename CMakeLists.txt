cmake_minimum_required(VERSION 3.10)

project(MazeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(MazeGame
    main.cpp
    Maze.cpp
    Interface.cpp
)

target_include_directories(MazeGame PRIVATE ${CURSES_INCLUDE_DIR})

target_link_libraries(MazeGame PRIVATE sfml-graphics sfml-window sfml-system)

target_compile_options(MazeGame PRIVATE -Wall -Wextra -Wpedantic)
