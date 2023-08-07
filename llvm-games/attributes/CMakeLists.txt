# CMake template file
#

cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/system/arm-none-eabi.cmake)

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

target_sources(${PROJECT_ELF} PRIVATE 
    main.c
    system/gcc_startup_nrf52840.S
    SEGGER/RTT/SEGGER_RTT.c
    SEGGER/RTT/SEGGER_RTT_printf.c
    SEGGER/RTT/SEGGER_RTT_ASM_ARMv7M.S
)

# List of compiler defines, prefix with -D compiler option
if(${USE_CLANG})
    target_compile_definitions(${PROJECT_ELF} PRIVATE
        # picolib does all this stuff of init
        -DNRF52840_XXAA
        #-D__STARTUP_CLEAR_BSS
        -D__STARTUP_SKIP_ETEXT
    )
else()
    target_compile_definitions(${PROJECT_ELF} PRIVATE
        # newlib needs at least copy of .data
        -DNRF52840_XXAA
        #-D__STARTUP_CLEAR_BSS
    )
endif()

# List of include directories
target_include_directories(${PROJECT_ELF} PRIVATE
    system/cmsis
    system/nordic
    SEGGER/Config
    .
)


#
# Compiler options
#
target_compile_options(${PROJECT_ELF} PRIVATE
        -fdata-sections
        -ffunction-sections
        -Wall
)
if(${USE_CLANG})
    target_compile_options(${PROJECT_ELF} PRIVATE
        --config ${TOOLCHAIN_CLANG_TARGET_CFG}
    )
else()
    target_compile_options(${PROJECT_ELF} PRIVATE
        -mcpu=${TOOLCHAIN_GCC_TARGET_CPU}
    )
endif()


#
# Linker options
#
target_link_options(${PROJECT_ELF} PRIVATE
        -fdata-sections
        -ffunction-sections
        -Wl,--defsym=__stack_size=${STACK_SIZE}
        -Wl,-Map=${PROJECT_ELF}.map
)
if(${USE_CLANG})
    target_link_options(${PROJECT_ELF} PRIVATE
        --config ${TOOLCHAIN_CLANG_TARGET_CFG}
        -T${CMAKE_SOURCE_DIR}/system/nrf52840-clang.ld
    )
else()
    target_link_options(${PROJECT_ELF} PRIVATE
        -mcpu=${TOOLCHAIN_GCC_TARGET_CPU}
        -T${CMAKE_SOURCE_DIR}/system/nrf52840-gcc.ld
        -specs=nano.specs                             # with nano 14K instead of 43K
        -specs=nosys.specs
    )
endif()


#
# Optional: Print executable size as part of the post build process
#
add_custom_command(TARGET ${PROJECT_ELF}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${PROJECT}.elf
)

#
# Optional: Create hex and bin files after the build
#
add_custom_command(TARGET ${PROJECT_ELF}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex   "${PROJECT}.elf" "${PROJECT}.hex"
        COMMAND ${CMAKE_OBJCOPY} -O binary "${PROJECT}.elf" "${PROJECT}.bin" 
)
