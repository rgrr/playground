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



/**
 * Directly call the SEGGER functions.
 */
int main(void)
{
    uint32_t cnt = 0;

    do {
        SEGGER_RTT_printf(0, "Hello, veryyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy long line %d\n", ++cnt);
        _Delay(50);
    } while (1);
    return 0;
}
