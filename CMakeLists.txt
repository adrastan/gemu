cmake_minimum_required(VERSION 3.7...3.21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

project(gemu VERSION 1.0 DESCRIPTION "Game Boy Emulator")

find_package(SDL2 REQUIRED)
include_directories(src2 src2/core ${SDL2_INCLUDE_DIRS})
add_executable(main src2/Main.cpp src2/core/System.cpp)
target_link_libraries(main PRIVATE SDL2::SDL2 -lmingw32 -mwindows)