/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdbool.h>
#include <SEGGER/RTT/SEGGER_RTT.h>


//----------------------------------------------------------------------------------------------------------------------
//
// misc utilities
//
extern uint32_t __stack;
extern uint32_t __my_data_end;


static void _Delay_us(uint32_t period_us)
{
    volatile int i;

    i = 6400 * (period_us / 1000);
    do {
        __asm volatile ("nop\n" : : :);
    } while (i--);
}   // _Delay_us


//----------------------------------------------------------------------------------------------------------------------
//
// various function attributes
//
typedef void (*VECTOR_FUNCTION_Type)(void);

void Interrupt_Handler(void) __attribute__ ((interrupt ("IRQ")));
void Noattribute_Handler(void);
void Naked_Handler(void) __attribute__ ((naked));
void Noreturn_Handler() __attribute__((__noreturn__, section(".third_stage_boot")));


const VECTOR_FUNCTION_Type test__VECTOR_TABLE[64] __attribute__((used, section(".test_vectors"))) = {
    Noreturn_Handler,
    Noattribute_Handler,
    Naked_Handler,
    Interrupt_Handler,
};


void Interrupt_Handler(void)
{
    __asm volatile ("nop\n" : : :);
}

void Noattribute_Handler(void)
{
    __asm volatile ("nop\n" : : :);
}

void Naked_Handler(void)
{
    __asm volatile ("nop\n" : : :);
}

void Noreturn_Handler(void)
{
    __asm volatile ("nop\n" : : :);
    for (;;) {
    }
}


static void print_functions_addresses(void)
{
    SEGGER_RTT_printf(0, "Interrupt_Handler   : %p\n", Interrupt_Handler);
    SEGGER_RTT_printf(0, "Noattribute_Handler : %p\n", Noattribute_Handler);
    SEGGER_RTT_printf(0, "Naked_Handler       : %p\n", Naked_Handler);
    SEGGER_RTT_printf(0, "Noreturn_Handler    : %p\n", Noreturn_Handler);
}   // print_functions_addresses


//----------------------------------------------------------------------------------------------------------------------


int main()
{
    SEGGER_RTT_printf(0, "\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    _Delay_us(1500 * 1000);
    SEGGER_RTT_printf(0, "Compiler:\n");
#if defined(__clang__)
    SEGGER_RTT_printf(0, "  clang %d.%d.%d (%s) -", __clang_major__, __clang_minor__, __clang_patchlevel__, __VERSION__);
#elif defined(__GNUC__)
    SEGGER_RTT_printf(0, "  gcc %d.%d.%d (%s) -", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__, __VERSION__);
#else
    SEGGER_RTT_printf(0, "  UNKNOWN - \n");
#endif
#if defined(__OPTIMIZE__)
    SEGGER_RTT_printf(0, " __OPTIMIZE__");
#endif
#if defined(__OPTIMIZE_SIZE__)
    SEGGER_RTT_printf(0, " __OPTIMIZE_SIZE__");
#endif
#if defined(__NO_INLINE__)
    SEGGER_RTT_printf(0, " __NO_INLINE__");
#endif
    SEGGER_RTT_printf(0, "\n");
    SEGGER_RTT_printf(0, "Compiled:\n  %s %s", __DATE__, __TIME__);
    SEGGER_RTT_printf(0, "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    SEGGER_RTT_printf(0, "\n========================================\n");
    SEGGER_RTT_printf(0, "Stack               : %p\n", &__stack);
    SEGGER_RTT_printf(0, "Data End            : %p\n", &__my_data_end);
    SEGGER_RTT_printf(0, "----------------------------------------\n");
    print_functions_addresses();
    SEGGER_RTT_printf(0, "========================================\n");

    while (true) {
    }
}
