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



#define SYSVIEW_DEVICE_NAME "PCA10056 Cortex-M4"
#define SYSVIEW_APP_NAME "SysView Games"


static void _cbSendSystemDesc(void) {
    SEGGER_SYSVIEW_SendSysDesc("N=" SYSVIEW_APP_NAME ",D=" SYSVIEW_DEVICE_NAME ",O=None");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}


void SEGGER_SYSVIEW_Conf(void)
{
    SEGGER_RTT_Init();
    SEGGER_SYSVIEW_Init(64000000, 64000000, NULL, _cbSendSystemDesc);
    SEGGER_SYSVIEW_SetRAMBase(0x20000000);
}   // SEGGER_SYSVIEW_Conf



int main()
{
    SEGGER_SYSVIEW_Conf();

    SEGGER_SYSVIEW_Start();
    //SEGGER_SYSVIEW_EnableEvents(0xffff);

    SEGGER_SYSVIEW_Error("Start\n");
    for (int i = 0;  i < 10;  ++i) {
        SEGGER_SYSVIEW_Warn("00000000000000000000000000000000000000000000000000\n");
        _Delay(222);
    }

    //SEGGER_SYSVIEW_DisableEvents(0xffff);
    SEGGER_SYSVIEW_Print("Stop\n");
    SEGGER_SYSVIEW_Stop();

    for (;;) {

    }
}   // main
