# Build commands
# --------------
# mkdir _build
# cd _build
# - rm -rf * &&             cmake -DCMAKE_BUILD_TYPE=Debug ..
# - rm -rf * &&             cmake -DCMAKE_BUILD_TYPE=Release ..
# - rm -rf * && CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Debug ..
# - rm -rf * && CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake --build . && time ./game-of-life
# 
cmake_minimum_required(VERSION 3.15)

project(GameOfLife
    VERSION
        1.0
    DESCRIPTION
        "Game Of Life"
    LANGUAGES
        CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
cmake_policy(SET CMP0135 NEW)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(game-of-life
    TLife_Tests.cpp
    TLife.cpp
    TLifeFaster.cpp
    TLifeFasterMulti.cpp
)
target_link_libraries(
    game-of-life
    GTest::gtest_main
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(game-of-life)
