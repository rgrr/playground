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

#include "SEGGER_SYSVIEW.h"



/*********************************************************************
 *
 *    Direct access to MCU registers
 *
 *********************************************************************/

#define SYS_CLOCK_HZ              64000000

#define DEMCR                     (*((volatile uint32_t *)0xE000EDFCuL))   // Debug Exception and Monitor Control Register
#define TRACEENA_BIT              (1uL << 24)                                   // Trace enable bit
#define DWT_CTRL                  (*((volatile uint32_t *)0xE0001000uL))   // DWT Control Register
#define NOCYCCNT_BIT              (1uL << 25)                                   // Cycle counter support bit
#define CYCCNTENA_BIT             (1uL << 0)                                    // Cycle counter enable bit
#define DWT_CYCCNT()              (*((volatile uint32_t *)0xE0001004))
#define SYSTICK                   ((SYSTICK_REGS *)0xE000E010)

typedef struct {
    volatile unsigned int CSR;
    volatile unsigned int RVR;
    volatile unsigned int CVR;
    volatile unsigned int CALIB;
} SYSTICK_REGS;


void arm_systick_isr(void)
{
    static volatile U32 Cnt = 0;
    SEGGER_SYSVIEW_RecordEnterISR();
    //for (int i = 0;  i < 10;  ++i)
        Cnt++;
    SEGGER_SYSVIEW_RecordExitISR();
}   // arm_systick_isr


void arm_systick_init(uint ips)
{
    SYSTICK->RVR = (SYS_CLOCK_HZ / ips) - 1;     // set reload
    SYSTICK->CVR = 0x00;      // set counter
    SYSTICK->CSR = 0x07;      // enable systick
}   // arm_systick_init


/*********************************************************************
 *
 *    Sysview
 *
 *********************************************************************/

#define SYSVIEW_DEVICE_NAME "PCA10056 Cortex-M4"
#define SYSVIEW_APP_NAME "SysView Games"


static void _cbSendSystemDesc(void) {
    SEGGER_SYSVIEW_SendSysDesc("N=" SYSVIEW_APP_NAME ",D=" SYSVIEW_DEVICE_NAME ",O=NoOS");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}


SEGGER_SYSVIEW_MODULE IPModule = {
    "M=TestSysView,"                       \
    "S=This just a test for SystemView,"   \
    "0 Print cnt=%d",                             // sModule
    2,                                            // NumEvents
    0,                                            // EventOffset, Set by SEGGER_SYSVIEW_RegisterModule()
    NULL,                                         // pfSendModuleDesc, NULL: No additional module description
    NULL,                                         // pNext, Set by SEGGER_SYSVIEW_RegisterModule()
};


void SEGGER_SYSVIEW_Conf(void)
{
    //
    // If no debugger is connected, the DWT must be enabled by the application
    //
    if ((DEMCR & TRACEENA_BIT) == 0) {
        DEMCR |= TRACEENA_BIT;
    }

    //
    //  The cycle counter must be activated in order
    //  to use time related functions.
    //
    if ((DWT_CTRL & NOCYCCNT_BIT) == 0) {           // Cycle counter supported?
        if ((DWT_CTRL & CYCCNTENA_BIT) == 0) {      // Cycle counter not enabled?
            DWT_CTRL |= CYCCNTENA_BIT;              // Enable Cycle counter
        }
    }

    SEGGER_SYSVIEW_Init(SYS_CLOCK_HZ, SYS_CLOCK_HZ, NULL, _cbSendSystemDesc);

    //SEGGER_SYSVIEW_RegisterModule( &IPModule);

    //arm_systick_init(10);
}   // SEGGER_SYSVIEW_Conf



#define MARKER_PRINT   0x2222
#define TASKID_PRINT   0x22
#define TASKID_DELAY   0x11


static void _Delay(int period)
{
    SEGGER_SYSVIEW_OnTaskStartExec(TASKID_DELAY);
    volatile int i = (600000 / (17*6+2)) * period + 100;
    do {
    } while (i--);
    SEGGER_SYSVIEW_OnTaskStopExec();
}   // _Delay



static void PrintCycCnt(int i)
{
    SEGGER_SYSVIEW_OnTaskStartExec(TASKID_PRINT);
    //SEGGER_SYSVIEW_RecordU32(IPModule.EventOffset + 0, i);
    SEGGER_SYSVIEW_WarnfTarget("cyccnt %d %u\n", i, DWT_CYCCNT());

    //
    // small demo, that even small runtime differences are visible, task runtimes:
    // - debug variant: 70.5µs .. 101.8µs
    // - optimized:     35.8µs ..  37.1µs
    //
    for (int dd = 0;  dd < 20*i;  ++dd)
    {
        __asm volatile ("nop\n" : : :);
    }
    SEGGER_SYSVIEW_OnTaskStopExec();
}   // PrintC



int main()
{
#if 1
    printf("012345678901234567123456789012345678901234567890123456789\n");
#endif

#if 1
    for (int i = 0;  i < 200;  ++i) {        // this delay is required to have a running CYCCNT after reset with pyocd
        _Delay(10);
    }
#endif

    printf("012345678901234567123456789012345678901234567890123456789\n");

    SEGGER_SYSVIEW_Conf();
    _Delay(100);
    SEGGER_SYSVIEW_Start();
    _Delay(100);
    SEGGER_SYSVIEW_NameMarker(0x2222, "Print");
    _Delay(100);

    //
    // name "tasks"
    //
    _Delay(100);
    {
        SEGGER_SYSVIEW_TASKINFO Info;

        SEGGER_SYSVIEW_OnTaskCreate(TASKID_PRINT);
        memset( &Info, 0, sizeof(Info));
        Info.TaskID = TASKID_PRINT;
        Info.sName = "PrintCycCnt";
        SEGGER_SYSVIEW_SendTaskInfo( &Info);
    }
    _Delay(100);
    {
        SEGGER_SYSVIEW_TASKINFO Info;

        SEGGER_SYSVIEW_OnTaskCreate(TASKID_DELAY);
        memset( &Info, 0, sizeof(Info));
        Info.TaskID = TASKID_DELAY;
        Info.sName = "Delay";
        SEGGER_SYSVIEW_SendTaskInfo( &Info);
    }

    _Delay(100);

    SEGGER_SYSVIEW_EnableEvents(0xffffffff);

    for (int j = 0;  j < 5000000;  ++j) {
        SEGGER_SYSVIEW_PrintfTarget("Start %d\n", j);
        SEGGER_SYSVIEW_OnIdle();
        for (int i = 0;  i < 5;  ++i) {
            SEGGER_SYSVIEW_MarkStart(MARKER_PRINT);
            SEGGER_SYSVIEW_OnIdle();
            PrintCycCnt(i);
            SEGGER_SYSVIEW_OnIdle();
            SEGGER_SYSVIEW_MarkStop(MARKER_PRINT);
            SEGGER_SYSVIEW_OnIdle();
            _Delay(0);
        }
#if 1
        if (j % 10 == 0)
            printf("0123456789012345678901234567890123456789 %d\n", j);
#endif
        _Delay(15);
    }

    SEGGER_SYSVIEW_DisableEvents(0xffffffff);
    SEGGER_SYSVIEW_Print("Stop\n");
    SEGGER_SYSVIEW_Stop();

    printf("finished\n");

    for (;;) {
        //SEGGER_SYSVIEW_IsStarted();
    }
}   // main
