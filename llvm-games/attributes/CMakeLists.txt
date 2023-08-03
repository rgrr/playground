# CMake template file
#

cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/system/arm-none-eabi-clang.cmake)

# Setup project, output and linker file
project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_ELF ${PROJECT}.elf)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

#########################################################
#
# Several options
#
set(STACK_SIZE  0x8000)

#
#########################################################

add_executable(${PROJECT_ELF})

set(SRC_FILES 
    main.c
    system/gcc_startup_nrf52840.S
    system/nordic/system_nrf52840.c
    SEGGER/RTT/SEGGER_RTT.c
    SEGGER/RTT/SEGGER_RTT_printf.c
    SEGGER/RTT/SEGGER_RTT_ASM_ARMv7M.S
)

target_sources(${PROJECT_ELF} PRIVATE ${SRC_FILES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${PROJECT_ELF} PRIVATE
    -DNRF52840_XXAA
    -D__STARTUP_SKIP_ETEXT
    -D__STARTUP_CLEAR_BSS          # actually not required, because done in crt0.c
)

# List of include directories
target_include_directories(${PROJECT_ELF} PRIVATE
    system/cmsis
    system/nordic
    SEGGER/Config
)

# Compiler options
target_compile_options(${PROJECT_ELF} PRIVATE
        --config ${TOOLCHAIN_TARGET_CFG}
        -fdata-sections
        -ffunction-sections
        -Wall
)

# Linker options
target_link_options(${PROJECT_ELF} PRIVATE
        --config ${TOOLCHAIN_TARGET_CFG}
        -fdata-sections
        -ffunction-sections
        -T${CMAKE_SOURCE_DIR}/system/nrf52840.ld
        -Wl,--defsym=__stack_size=${STACK_SIZE}
        -Wl,-Map=${PROJECT_ELF}.map
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
