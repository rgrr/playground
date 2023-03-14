
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#include "InstrProfiling.h"
#include "InstrProfilingInternal.h"
#include "InstrProfilingPort.h"
#include "InstrProfilingUtil.h"



void __llvm_profile_initialize_file(void)
{
}   // __llvm_profile_initialize_file



int __llvm_profile_write_file(void)
{
    return 0;
}   // __llvm_profile_write_file



/**
 * This method is invoked by the runtime initialization hook
 * InstrProfilingRuntime.o if it is linked in.
 */
COMPILER_RT_VISIBILITY void __llvm_profile_initialize(void)
{
    __llvm_profile_initialize_file();
    __llvm_profile_register_write_file_atexit();
}   // __llvm_profile_initialize



static void writeFileWithoutReturn(void)
{
    __llvm_profile_write_file();
}   // writeFileWithoutReturn



COMPILER_RT_VISIBILITY int __llvm_profile_register_write_file_atexit(void)
{
    static bool HasBeenRegistered = false;

    if (HasBeenRegistered)
        return 0;
    HasBeenRegistered = true;

    return atexit(writeFileWithoutReturn);
}   // __llvm_profile_register_write_file_atexit


