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

if(WIN32)
    set(EXE_SUFFIX ".exe" )
else()
    set(EXE_SUFFIX "" )
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# specify the cross compiler
set(CMAKE_C_COMPILER "${TOOLCHAIN_PATH}/clang${EXE_SUFFIX}")
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/clang++${EXE_SUFFIX}")

SET(CMAKE_C_COMPILER_TARGET   ${TOOLCHAIN_TARGET})
SET(CMAKE_CXX_COMPILER_TARGET ${TOOLCHAIN_TARGET})
SET(CMAKE_ASM_COMPILER_TARGET ${TOOLCHAIN_TARGET})

set(CMAKE_OBJCOPY   ${TOOLCHAIN_PATH}/llvm-objcopy${EXE_SUFFIX} CACHE INTERNAL "llvm objcopy tool")
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PATH}/llvm-size${EXE_SUFFIX}    CACHE INTERNAL "llvm size tool")
set(CMAKE_LINKER    ${TOOLCHAIN_PATH}/ld.ldd${EXE_SUFFIX}       CACHE INTERNAL "llvm ARM linker")
set(CMAKE_AR        ${TOOLCHAIN_PATH}/llvm-ar${EXE_SUFFIX}      CACHE INTERNAL "llvm ARM archiver")

set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PATH}/..)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
