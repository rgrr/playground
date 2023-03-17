
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <fcntl.h>


#include "InstrProfiling.h"
#include "InstrProfilingInternal.h"
#include "InstrProfilingPort.h"
#include "InstrProfilingUtil.h"



#define FILE_NAME   "default.profraw"



/* Return 1 if there is an error, otherwise return  0.  */
static uint32_t fileWriter(ProfDataWriter *This, ProfDataIOVec *IOVecs, uint32_t NumIOVecs)
{
    uint32_t I;
    int fd = (int)This->WriterCtx;
    char Zeroes[sizeof(uint64_t)] = {0};
    for (I = 0; I < NumIOVecs; I++)
    {
        size_t Length = IOVecs[I].ElmSize * IOVecs[I].NumElm;

        if (IOVecs[I].Data)
        {
            if (write(fd, IOVecs[I].Data, Length) != Length)
                return 1;
        }
        else if (IOVecs[I].UseZeroPadding)
        {
            while (Length > 0)
            {
                size_t PartialWriteLen = (sizeof(uint64_t) > Length) ? Length : sizeof(uint64_t);
                if (write(fd, Zeroes, PartialWriteLen) != PartialWriteLen)
                {
                    return 1;
                }
                Length -= PartialWriteLen;
            }
        }
    }
    return 0;
}



static void initFileWriter(ProfDataWriter *This, int fd)
{
    This->Write = fileWriter;
    This->WriterCtx = (void *)fd;
}   // initFileWriter



void __llvm_profile_initialize_file(void)
{
}   // __llvm_profile_initialize_file



int __llvm_profile_write_file(void)
{
    ProfDataWriter fileWriter;
    int fd;
    int r;

    fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    initFileWriter(&fileWriter, fd);
    r = lprofWriteData(&fileWriter, 0, 0);
    close(fd);
    return r;
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


