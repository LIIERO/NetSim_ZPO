cmake_minimum_required(VERSION 3.20)

project(NetSim_ZPO)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(include)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp)


add_executable(NetSim_ZPO ${SOURCE_FILES} main.cpp)



set(SOURCES_FILES_TESTS
        test/test_package.cpp
        test/test_Package_queue.cpp
        )

add_executable(NetSim_ZPO__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

# TODO: Poniższe ścieżki prawdopodobnie musisz zmienić...
target_include_directories(NetSim_ZPO__test PUBLIC
        ../googletest-master/googlemock/include
        ../googletest-master/googletest/include
        )

# TODO: Poniższą ścieżkę prawdopodobnie musisz zmienić...
add_subdirectory(../googletest-master googletest-master)

target_link_libraries(NetSim_ZPO__test gmock)

