add_library(rt-profile 
        InstrProfiling.c
        InstrProfilingBuffer.c
        InstrProfilingFileOther.c
        InstrProfilingInternal.c
        InstrProfilingPlatformOther.c
        InstrProfilingRuntime.cpp
        InstrProfilingVersionVar.c
        InstrProfilingWriter.c
)

target_compile_options(rt-profile PRIVATE
        --config armv6m_soft_nofp_semihost.cfg
        -fdata-sections -ffunction-sections
        -Wall
)
