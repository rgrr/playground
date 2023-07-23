add_library(rt-profile 
        InstrProfiling.c
        InstrProfilingBuffer.c
        InstrProfilingFileSemihosting.c
        InstrProfilingInternal.c
        InstrProfilingVersionVar.c
        InstrProfilingWriter.c
)

target_compile_options(rt-profile PRIVATE
        --config armv7em_soft_nofp_semihost.cfg
        -fdata-sections -ffunction-sections
        -Wall
)
