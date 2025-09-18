cmake_minimum_required(VERSION 3.15)
project(manochip)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FILES src/main.cpp src/cpu.cpp src/cpu.h)

add_subdirectory(lib/raylib)
include_directories(lib/raylib/src)

file(GLOB SRC_FILES src/*.cpp src/*.h)

add_executable(game ${SRC_FILES})

target_include_directories(game PRIVATE 
    src
    lib/raylib/src
    lib/raylib/src/raygui
)

target_link_libraries(game PRIVATE raylib)