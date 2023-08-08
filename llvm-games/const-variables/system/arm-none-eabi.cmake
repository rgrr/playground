# see https://cmake.org/cmake/help/book/mastering-cmake/chapter/Cross%20Compiling%20With%20CMake.html
#
# Call Cmake from the 'build' subfolder with the command below.
# For using Make:
# cmake -DCMAKE_MAKE_PROGRAM=make.exe -DCMAKE_TOOLCHAIN_FILE="arm-none-eabi-gcc.cmake" -G "Unix Makefiles" ..
# followed by
# 'make' or 'cmake --build .' to build it

set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(${USE_CLANG})
    # specify the cross compiler
    set(CMAKE_C_COMPILER   "${TOOLCHAIN_PATH}/clang")
    set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
    set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/clang++")
    
    set(CMAKE_OBJCOPY   ${TOOLCHAIN_PATH}/llvm-objcopy CACHE INTERNAL "llvm objcopy tool")
    set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PATH}/llvm-size    CACHE INTERNAL "llvm size tool")
    set(CMAKE_LINKER    ${TOOLCHAIN_PATH}/ld.ldd       CACHE INTERNAL "llvm ARM linker")
    set(CMAKE_AR        ${TOOLCHAIN_PATH}/llvm-ar      CACHE INTERNAL "llvm ARM archiver")
    set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PATH}/..)
else()
    # specify the cross compiler
    set(CMAKE_C_COMPILER   "arm-none-eabi-gcc")
    set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
    set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
    
    set(CMAKE_OBJCOPY   arm-none-eabi-objcopy CACHE INTERNAL "gcc objcopy tool")
    set(CMAKE_SIZE_UTIL arm-none-eabi-size    CACHE INTERNAL "gcc size tool")
    set(CMAKE_LINKER    arm-none-eabi-ld      CACHE INTERNAL "gcc ARM linker")
    set(CMAKE_AR        arm-none-eabi-ar      CACHE INTERNAL "gcc ARM archiver")
    # set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PATH}/..)
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
