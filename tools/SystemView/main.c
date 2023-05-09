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

//#include "SEGGER_RTT.h"
#include "SEGGER_SYSVIEW.h"



#define SYSVIEW_DEVICE_NAME "PCA10056 Cortex-M4"
#define SYSVIEW_APP_NAME "SysView Games"


/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
#define KIN1_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
  /*!< DWT Control register */
#define KIN1_DWT_CYCCNTENA_BIT       (1UL<<0)
  /*!< CYCCNTENA bit in DWT_CONTROL register */
#define KIN1_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
  /*!< DWT Cycle Counter register */
#define KIN1_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))
  /*!< DEMCR: Debug Exception and Monitor Control Register */
#define KIN1_TRCENA_BIT              (1UL<<24)
  /*!< Trace enable bit in DEMCR register */


#define KIN1_InitCycleCounter()      KIN1_DEMCR |= KIN1_TRCENA_BIT
  /*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */

#define KIN1_ResetCycleCounter()     KIN1_DWT_CYCCNT = 0
  /*!< Reset cycle counter */

#define KIN1_EnableCycleCounter()    KIN1_DWT_CONTROL |= KIN1_DWT_CYCCNTENA_BIT
  /*!< Enable cycle counter */

#define KIN1_DisableCycleCounter()   KIN1_DWT_CONTROL &= ~KIN1_DWT_CYCCNTENA_BIT
  /*!< Disable cycle counter */

#define KIN1_GetCycleCounter()       KIN1_DWT_CYCCNT
  /*!< Read cycle counter register */



static void _cbSendSystemDesc(void) {
    SEGGER_SYSVIEW_SendSysDesc("N=" SYSVIEW_APP_NAME ",D=" SYSVIEW_DEVICE_NAME ",O=None");
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
    KIN1_InitCycleCounter();
    KIN1_ResetCycleCounter();
    KIN1_EnableCycleCounter();

    SEGGER_SYSVIEW_Init(64000000, 64000000, NULL, _cbSendSystemDesc);

    SEGGER_SYSVIEW_RegisterModule( &IPModule);
}   // SEGGER_SYSVIEW_Conf



#define MARKER_PRINT   0x2222
#define TASKID_PRINT   0x22
#define TASKID_DELAY   0x11


static void _Delay(int period)
{
    SEGGER_SYSVIEW_OnTaskStartExec(TASKID_DELAY);
    volatile int i = (600000 / (17*6+2)) * period + 100;
    do {
        //SEGGER_SYSVIEW_IsStarted();
    } while (i--);
    SEGGER_SYSVIEW_OnTaskStopExec();
}   // _Delay



static void PrintCycCnt(int i)
{
    SEGGER_SYSVIEW_OnTaskStartExec(TASKID_PRINT);
    SEGGER_SYSVIEW_RecordU32(IPModule.EventOffset + 0, i);
    SEGGER_SYSVIEW_WarnfTarget("cyccnt %d %u\n", i, KIN1_GetCycleCounter());

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
    //printf("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n");

    // TODO hierdurch wird die Probe gekillt
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

    SEGGER_SYSVIEW_EnableEvents(0xffff);

    for (int j = 0;  j < 5000000;  ++j) {
        SEGGER_SYSVIEW_PrintfTarget("Start %d\n", j);
        SEGGER_SYSVIEW_OnIdle();
        for (int i = 0;  i < 5;  ++i) {
            //SEGGER_SYSVIEW_RecordU32(0x99, KIN1_GetCycleCounter());
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

    SEGGER_SYSVIEW_DisableEvents(0xffff);
    SEGGER_SYSVIEW_Print("Stop\n");
    SEGGER_SYSVIEW_Stop();

    printf("finished\n");

    for (;;) {
        //SEGGER_SYSVIEW_IsStarted();
    }
}   // main
