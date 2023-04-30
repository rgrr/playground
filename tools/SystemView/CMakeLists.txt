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

set(PROJECT_ELF ${PROJECT}.elf)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS        OFF)

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

# List of source files
set(SRC_FILES
        main.c
)

# Build the executable based on the source files
add_executable(${PROJECT_ELF} ${SRC_FILES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${PROJECT_ELF} PRIVATE
)

# List of include directories
target_include_directories(${PROJECT_ELF} PRIVATE
)

########################################################################################################################
#
# Some configurations
#
#
#if(0)
#    # profiling function from here
#    add_subdirectory(profiling)
#    target_link_libraries(${PROJECT_ELF} LINK_PUBLIC 
#            rt-profile
#    )
#else()
#    # profiling function from generated libclang_rt.profile.a
#    target_link_libraries(${PROJECT_ELF} LINK_PUBLIC 
#            clang_rt.profile
#    )
#endif()

#
# if no semihosting is used, __llvm_profile_write_file() must be called at end of main()
# use either armv6m_soft_nofp_semihost.cfg or armv6m_soft_nofp.cfg
#
set(CONFIG armv7em_soft_nofp_semihost.cfg)

# -> __cyg_profile_func_enter() and __cyg_profile_func_exit()
# Instrument device library for HIP, which is a LLVM bitcode containing __cyg_profile_func_enter and __cyg_profile_func_exit
set(XRAY_CONFIG -finstrument-functions)

# -> __cyg_profile_func_enter_bare()
#set(XRAY_CONFIG -finstrument-function-entry-bare)

# -> nothing required!?
#set(XRAY_CONFIG -pg)

# regular profiling
#set(XRAY_CONFIG -fprofile-instr-generate -fcoverage-mapping)

# -> does not work for thumb
#set(XRAY_CONFIG -fxray-instrument)

# -> does not work
#set(XRAY_CONFIG -p)

#
########################################################################################################################

# Compiler options
target_compile_options(${PROJECT_ELF} PRIVATE
        --config ${CONFIG}
        -fdata-sections -ffunction-sections
        ${XRAY_CONFIG}
        -v
        -Wall
)

# Linker options
target_link_options(${PROJECT_ELF} PRIVATE
        --config ${CONFIG}
        -fdata-sections -ffunction-sections
        ${XRAY_CONFIG}
        -T${CMAKE_SOURCE_DIR}/nrf52840.ld
        -Wl,-Map=${PROJECT_ELF}.map
        -v
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
