set(LIB_SYSVIEW sysview)

add_library(${LIB_SYSVIEW}
        Sysview/SEGGER_RTT.c
        Sysview/SEGGER_RTT_printf.c
        Sysview/SEGGER_RTT_ASM_ARMv7M.S
        Sysview/SEGGER_SYSVIEW.c
)

target_compile_options(${LIB_SYSVIEW} PRIVATE
        --config armv7em_soft_nofp.cfg
        -fdata-sections -ffunction-sections
        -Wall
)

target_include_directories(${LIB_SYSVIEW} PRIVATE
    Config
    Sysview
)
