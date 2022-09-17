# This is the top CMakeLists.txt.
#
# Everything else is included from sub-CMakeLists.txt
#
cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

project(Playground LANGUAGES C CXX)

# meant is the project root
get_filename_component(ROOT_DIR         "${CMAKE_CURRENT_LIST_DIR}"                ABSOLUTE)

# we want at least C++17
set(CMAKE_CXX_STANDARD 17)


include(CTest)
include(cmake/macros.cmake)


#
# currently just for test
#
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(CMAKE_C_FLAGS   "-Wall -g")
    set(CMAKE_CXX_FLAGS "-Wall -g")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    set(CMAKE_C_FLAGS   "-Wall --coverage -O0 -g")
    set(CMAKE_CXX_FLAGS "-Wall --coverage -O0 -g")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
endif()


add_subdirectory(games)
add_subdirectory(unittest)
