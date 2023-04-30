/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if 0
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include "SEGGER_RTT.h"
#include "SEGGER_SYSVIEW.h"



static void _Delay(int period)
{
    volatile int i = (100000 / 17) * period;
    do {
        ;
    } while (i--);
}   // _Delay



void SEGGER_SYSVIEW_Conf(void)
{

}   // SEGGER_SYSVIEW_Conf



int main()
{
    SEGGER_RTT_Init();
    SEGGER_RTT_printf(0, "Hello Sysview\n");

#if 0
    SEGGER_SYSVIEW_Conf();

    SEGGER_SYSVIEW_Start();

    for (int i = 0;  i < 10;  ++i) {
        _Delay(122);
    }

    SEGGER_SYSVIEW_Stop();
#endif

    for (;;) {

    }
}   // main
#else
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
#endif
