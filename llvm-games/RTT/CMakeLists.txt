# CMake template file
#

cmake_minimum_required(VERSION 3.15.3)

# 0: call SEGGER_RTT_printf() directly
# 1: use picolib stdio
# 2: use librtt
if(NOT RTT_TYPE)
    set(RTT_TYPE 0)
endif()


# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-clang.cmake)

# Setup project, output and linker file
project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_ELF ${PROJECT}.elf)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

#
# depending on RTT_TYPE use different source files
#
message(STATUS "RTT_TYPE is: ${RTT_TYPE}")
add_executable(${PROJECT_ELF})
if(RTT_TYPE EQUAL 0)
    set(SRC_FILES main-0-rtt-printf.cpp)
    add_subdirectory(SEGGER)
    target_link_libraries(${PROJECT_ELF} LINK_PUBLIC RTT)
elseif(RTT_TYPE EQUAL 1)
    set(SRC_FILES main-1-rtt-picolib-stdio.cpp)
    add_subdirectory(SEGGER)
    target_link_libraries(${PROJECT_ELF} LINK_PUBLIC RTT)
elseif(RTT_TYPE EQUAL 2)
    set(SRC_FILES main-2-librtt.cpp)
    target_link_libraries(${PROJECT_ELF} librtt_stdio.a librtt.a)
endif()

# TODO is there a better way?
target_sources(${PROJECT_ELF} PRIVATE ${SRC_FILES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${PROJECT_ELF} PRIVATE
)

# List of include directories
target_include_directories(${PROJECT_ELF} PRIVATE
)

# TODO armv7em_soft_nofp kills compilation!  But for other projects it works.

# Compiler options
target_compile_options(${PROJECT_ELF} PRIVATE
        --config armv7m_soft_nofp.cfg
        -fdata-sections
        -ffunction-sections
        -Wall
)

# Linker options
target_link_options(${PROJECT_ELF} PRIVATE
        --config armv7m_soft_nofp.cfg
        -fdata-sections
        -ffunction-sections
        -T${CMAKE_SOURCE_DIR}/nrf52840.ld
)

# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${PROJECT_ELF}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${PROJECT}.elf
)

# Optional: Create hex and bin files after the build
add_custom_command(TARGET ${PROJECT_ELF}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex   "${PROJECT}.elf" "${PROJECT}.hex"
        COMMAND ${CMAKE_OBJCOPY} -O binary "${PROJECT}.elf" "${PROJECT}.bin" 
)
