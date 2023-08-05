/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdbool.h>
#include <SEGGER/RTT/SEGGER_RTT.h>


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


#define INC_CONST

#ifdef INC_CONST
uint8_t u0[]              = "uint8_t []";
static uint8_t u1[]       = "static uint8_t *";
const uint8_t u2[]        = "const uint8_t *";
static const uint8_t u3[] = "static const uint8_t *";
extern uint32_t __stack;
extern uint32_t __my_data_end;


static char const *p_eval( uint8_t const * const p )
{
    if (p >= (uint8_t *)&__my_data_end) {
        return "STACK";
    }
    else if (p >= (uint8_t *)0x20000000) {
        return "RAM";
    }
    return "FLASH";
}


static void p_u( uint8_t *p )
{
    SEGGER_RTT_printf(0, "  p_u('%s')=0x%08p - %s\n", (char *)p, p, p_eval(p));
}

static void constp_u( uint8_t * const p )
{
    SEGGER_RTT_printf(0, "  constp_u('%s')=0x%08p - %s\n", (char *)p, p, p_eval(p));
}

static void p_constu( uint8_t const *p )
{
    SEGGER_RTT_printf(0, "  p_constu('%s')=0x%08p - %s\n", (char *)p, p, p_eval(p));
}

static void constp_constu( uint8_t const * const p )
{
    SEGGER_RTT_printf(0, "  constp_constu('%s')=0x%08p - %s\n", (char *)p, p, p_eval(p));
}
#endif



static void _Delay_us(uint32_t period_us)
{
    volatile int i;

    i = 6400 * (period_us / 1000);
    do {
        __asm volatile ("nop\n" : : :);
    } while (i--);
}   // _Delay_us



#if !defined(__clang_major__)
// actually required for newlib
void _exit(void)
{
    for (;;) {
    }
}   // _exit
#endif


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
    SEGGER_RTT_printf(0, "Interrupt_Handler   : %p\n", Interrupt_Handler);
    SEGGER_RTT_printf(0, "Noattribute_Handler : %p\n", Noattribute_Handler);
    SEGGER_RTT_printf(0, "Naked_Handler       : %p\n", Naked_Handler);
    SEGGER_RTT_printf(0, "Noreturn_Handler    : %p\n", Noreturn_Handler);
    SEGGER_RTT_printf(0, "========================================\n");

#ifdef INC_CONST
    _Delay_us(200 * 1000);
    p_u(u0);
    p_u(u1);

    _Delay_us(200 * 1000);
    constp_u(u0);
    constp_u(u1);

    _Delay_us(200 * 1000);
    p_constu(u0);
    p_constu(u1);
    p_constu(u2);
    p_constu(u3);

    _Delay_us(200 * 1000);
    constp_constu(u0);
    constp_constu(u1);
    constp_constu(u2);
    constp_constu(u3);
#endif
    SEGGER_RTT_printf(0, "========================================\n");

    while (true) {
        _Delay_us(250 * 1000);
    }
}
