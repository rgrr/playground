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



uint8_t u0[]              = "uint8_t []";
static uint8_t u1[]       = "static uint8_t *";
const uint8_t u2[]        = "const uint8_t *";
static const uint8_t u3[] = "static const uint8_t *";


static char const *p_eval( uint8_t const * const p )
{
    extern uint32_t __stack;

    if (p >= (uint8_t *)&__stack) {
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



static void _Delay(uint32_t period_us)
{
    volatile int i;

    i = (6400 * period_us) / 1000;
    do {
        __asm volatile ("nop\n" : : :);
    } while (i--);
}   // _Delay


//void Interrupt_Handler(void) __attribute__ ((interrupt ("IRQ")));
//void Noattribute_Handler(void);
//void Naked_Handler(void) __attribute__ ((naked));
//void Noreturn_Handler() __attribute__((__noreturn__, section(".third_stage_boot")));

int main()
{
    SEGGER_RTT_printf(0, "Interrupt_Handler   : %p\n", Interrupt_Handler);
    SEGGER_RTT_printf(0, "Noattribute_Handler : %p\n", Noattribute_Handler);
    SEGGER_RTT_printf(0, "Naked_Handler       : %p\n", Naked_Handler);
    SEGGER_RTT_printf(0, "Noreturn_Handler    : %p\n", Noreturn_Handler);

    p_u(u0);
    constp_u(u1);
    p_constu(u2);
    constp_constu(u3);

    while (true) {
        _Delay(250 * 1000);
    }
}
