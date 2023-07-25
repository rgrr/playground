add_library(RTT
        RTT/SEGGER_RTT_printf.c
        RTT/SEGGER_RTT.c
        RTT/SEGGER_RTT_ASM_ARMv7M.S
)

target_compile_options(RTT PRIVATE
        --config ${TOOLCHAIN_TARGET}.cfg
        -fdata-sections -ffunction-sections
        -Wall
)


target_include_directories(RTT PRIVATE
    Config
)
