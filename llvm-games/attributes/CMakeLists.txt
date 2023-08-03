# Boilerplate CMakeLists.txt
#
# inputs
# ------
# PROJECT     - project name
# PICO_BOARD  - pico or pico_w
#

cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add your source files
add_executable(${PROJECT}
    main.c # <-- Add source files here!
)


target_link_libraries(${PROJECT} PRIVATE pico_stdlib)

pico_enable_stdio_uart(${PROJECT} 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT})
