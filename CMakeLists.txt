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

target_link_libraries(MazeGame PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_options(MazeGame PRIVATE -Wall -Wextra -Wpedantic)

target_compile_definitions(MazeGame PRIVATE COMPLETION_FONT_FILENAME="completion.ttf")

if(UNIX AND NOT APPLE)
    find_file(MAZEGAME_COMPLETION_FONT
        NAMES DejaVuSans.ttf LiberationSans-Regular.ttf Arial.ttf
        PATHS
            /usr/share/fonts/truetype/dejavu
            /usr/share/fonts/truetype/liberation2
            /usr/share/fonts/truetype/msttcorefonts
            /usr/share/fonts/TTF
            /usr/share/fonts
        NO_DEFAULT_PATH
    )

    if(MAZEGAME_COMPLETION_FONT)
        add_custom_command(TARGET MazeGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MAZEGAME_COMPLETION_FONT}"
                    "$<TARGET_FILE_DIR:MazeGame>/completion.ttf"
        )
    else()
        message(WARNING "Linux font was not found. Put completion.ttf next to the MazeGame executable.")
    endif()
endif()
