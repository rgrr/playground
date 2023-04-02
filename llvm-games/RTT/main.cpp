/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include "SEGGER/RTT/SEGGER_RTT.h"



static void _Delay(int period)
{
    volatile int i = (100000 / 17) * period;
    do {
        ;
    } while (i--);
}   // _Delay



int main(void)
{
    uint32_t cnt = 0;

    do {
        //SEGGER_RTT_WriteString(0, "__Hello World from SEGGER!\n");
        SEGGER_RTT_printf(0, "Hello %d\n", ++cnt);
        _Delay(1);
    } while (1);
    return 0;
}
