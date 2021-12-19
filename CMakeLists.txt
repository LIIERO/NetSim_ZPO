cmake_minimum_required(VERSION 3.20)

project(NetSim_ZPO)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(include)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp)


add_executable(NetSim ${SOURCE_FILES} main.cpp)
