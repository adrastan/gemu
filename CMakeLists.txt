cmake_minimum_required(VERSION 3.7...3.21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

project(gemu VERSION 1.0 DESCRIPTION "Game Boy Emulator")

find_package(SDL2 REQUIRED)

# Module: utilities
add_library(
    gemu_lib
    src/utils/logger.cpp
    src/utils/logger.h
    src/utils/file.cpp
    src/utils/file.h
    src/utils/bit_helper.cpp
    src/utils/bit_helper.h
)
target_include_directories(gemu_lib PUBLIC src/utils)

# Module: core
add_library(
    gemu_core
    src/core/system_types.h
    src/core/system.cpp
    src/core/system.h
    src/core/cartridge.cpp
    src/core/cartridge.h
)
target_link_libraries(gemu_core SDL2::SDL2 gemu_lib -lmingw32)
target_include_directories(gemu_core PRIVATE src/core ${SDL2_INCLUDE_DIRS})

add_executable(main src/main.cpp)
target_link_libraries(main gemu_core)
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
