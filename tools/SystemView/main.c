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
    SEGGER_RTT_Init();
}   // SEGGER_SYSVIEW_Conf



int main()
{
    SEGGER_SYSVIEW_Conf();

    SEGGER_SYSVIEW_Start();

    for (int i = 0;  i < 10;  ++i) {
        _Delay(122);
    }

    SEGGER_SYSVIEW_Stop();

    for (;;) {

    }
}   // main
