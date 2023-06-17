/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>



static inline void set_r12(uint32_t v)
{
    register uint32_t reg __asm("r12") = v;

    __asm volatile (
        "\n"
        :                                           /* output */
        : "r" (reg)                                 /* inputs */
                                                    /* nothing clobbered */
    );
}   // set_r12



static inline uint32_t get_r12(void)
{
    register uint32_t reg __asm("r12");

    __asm volatile (
        "\n"
        : "=r" (reg)                                /* output */
        :                                           /* inputs */
                                                    /* nothing clobbered */
    );
    return reg;
}   // get_r12



int main()
{
    set_r12(0xaaaa5555);
    printf("Hello world %d\n", get_r12());
    set_r12(0x99aabbcc);
    printf("Hello world %d\n", get_r12());
}
