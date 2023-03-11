# CMake template file
#

cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-clang.cmake)

# Setup project, output and linker file
project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(BUILD_FOLDER ./_build)
set(EXECUTABLE ${PROJECT}.elf)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/_build)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

# List of source files
set(SRC_FILES
        main.cpp
        )

# Build the executable based on the source files
add_executable(${EXECUTABLE} ${SRC_FILES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
        )

# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
        )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
        --config armv6m_soft_nofp_semihost.cfg
        -fdata-sections
        -ffunction-sections
        -Wall
        -g3
        )

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
        --config armv6m_soft_nofp_semihost.cfg
        -T../nrf52840.ld
        -lc
        -lm
        -g3
        )

# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE})

# Optional: Create hex and bin files after the build
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex "${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE}" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT}.hex"
        COMMAND ${CMAKE_OBJCOPY} -O binary "${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE}" "${EXECUTABLE_OUTPUT_PATH}/${PROJECT}.bin" 
        )
