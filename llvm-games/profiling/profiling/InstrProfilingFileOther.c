
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <fcntl.h>


#include "InstrProfiling.h"
#include "InstrProfilingInternal.h"
#include "InstrProfilingPort.h"
#include "InstrProfilingUtil.h"



#define FILE_NAME   "llvm-games/profiling/default.profraw"



/* Return 1 if there is an error, otherwise return  0.  */
static uint32_t fileWriter(ProfDataWriter *This, ProfDataIOVec *IOVecs, uint32_t NumIOVecs)
{
    uint32_t I;
    FILE *File = (FILE *)This->WriterCtx;
    char Zeroes[sizeof(uint64_t)] = {0};
    for (I = 0; I < NumIOVecs; I++)
    {
        if (IOVecs[I].Data)
        {
            if (fwrite(IOVecs[I].Data, IOVecs[I].ElmSize, IOVecs[I].NumElm, File) != IOVecs[I].NumElm)
                return 1;
        }
        else if (IOVecs[I].UseZeroPadding)
        {
            size_t BytesToWrite = IOVecs[I].ElmSize * IOVecs[I].NumElm;
            while (BytesToWrite > 0)
            {
                size_t PartialWriteLen = (sizeof(uint64_t) > BytesToWrite) ? BytesToWrite : sizeof(uint64_t);
                if (fwrite(Zeroes, sizeof(uint8_t), PartialWriteLen, File) != PartialWriteLen)
                {
                    return 1;
                }
                BytesToWrite -= PartialWriteLen;
            }
        }
        else
        {
            if (fseek(File, IOVecs[I].ElmSize * IOVecs[I].NumElm, SEEK_CUR) == -1)
                return 1;
        }
    }
    return 0;
}



static void initFileWriter(ProfDataWriter *This, FILE *File)
{
  This->Write = fileWriter;
  This->WriterCtx = File;
}



void __llvm_profile_initialize_file(void)
{
}   // __llvm_profile_initialize_file



int __llvm_profile_write_file(void)
{
    ProfDataWriter fileWriter;
    FILE *f;
    int r;

    f = fopen(FILE_NAME, "wb");
    initFileWriter(&fileWriter, f);
    r = lprofWriteData(&fileWriter, 0, 0);
    fclose(f);
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


