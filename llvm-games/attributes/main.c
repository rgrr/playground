/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>
#include <pico/stdlib.h>


// starter from https://github.com/JustAnother1/nomagic_probe/blob/main/src/hal/startup.c
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



static void led(uint8_t state)
{
}   // led


int main()
{
    uint32_t cnt = 0;

    stdio_init_all();
    printf("Hello %s\n", PICO_TARGET_NAME);

    while (true) {
        printf("-- %u\n", cnt++);
        led(1);
        sleep_ms(250);
        led(0);
        sleep_ms(250);
    }
}
