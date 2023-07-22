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

#include "nrf.h"

#include "SEGGER/RTT/SEGGER_RTT.h"
#include "SEGGER/RTT/SEGGER_SYSVIEW.h"



/*********************************************************************
 *
 *    Direct access to MCU registers
 *
 *********************************************************************/

void SysTick_Handler(void)
{
    static volatile U32 Cnt = 0;
    SEGGER_SYSVIEW_RecordEnterISR();
//    for (int i = 0;  i < 1000;  ++i)
        Cnt++;
    SEGGER_SYSVIEW_RecordExitISR();
}   // SysTick_Handler


void SysTick_Init(uint32_t ips)
/**
 * Init SysTick interrupt.
 * SysTick interrupt is also set to lowest priority.
 *
 * \note
 *    nRF52840 has 3 bits for priority level.  __NVIC_SetPriority() sets the upper bits of an 8bit register.
 */
{
    SysTick_Config((SystemCoreClock / ips) - 1);
}   // SysTick_Init


/*********************************************************************
 *
 *    Sysview
 *
 *    To give the SEGGER_SYSVIEW_Record*() calls symbolic names and correct parameter printout,
 *    define /opt/SEGGER/SystemView_V352/Description/SYSVIEW_NoOS.txt with the following content:

33    Delay                               delay_us=%u
50    _TestFunc0_STATE_INIT
51    _TestFunc0_STATE_WAIT_FOR_DATA
52    _TestFunc0_STATE_PROCESS_DATA
53    _TestFunc0_STATE_SEND_DATA

    OR

33    Delay                               delay_us=%Time
50    _TestFunc0_STATE_INIT
51    _TestFunc0_STATE_WAIT_FOR_DATA
52    _TestFunc0_STATE_PROCESS_DATA
53    _TestFunc0_STATE_SEND_DATA

NamedType Time 0=NULL 200=0.2ms 500=0.5ms
NamedType Time 1000=1ms 40000=40ms
NamedType Time 20=20us 30=30us 40=40us 50=50us

 *
 *********************************************************************/

#define SYSVIEW_APP_NAME    "SysView Games"
#define SYSVIEW_DEVICE_NAME "PCA10056 nRF52840"
#define SYSVIEW_CORE_NAME   "Cortex-M4"
#define SYSVIEW_OS_NAME     "NoOS"                 // this decides about the above name

#define MARKER_PRINT        1
#define MARKER_PRINT_MOD    3
#define TASKID_CYCLIC       0x22
#define TASKID_DELAY        0x11
#define TASKID_TESTFUNC0    0x99
#define TASKID_MAINLOOP     0x1234


static void _cbSendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=" SYSVIEW_APP_NAME);
    SEGGER_SYSVIEW_SendSysDesc("D=" SYSVIEW_DEVICE_NAME);
    SEGGER_SYSVIEW_SendSysDesc("C=" SYSVIEW_CORE_NAME);
    SEGGER_SYSVIEW_SendSysDesc("O=" SYSVIEW_OS_NAME);
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}   // _cbSendSystemDesc


static void _cbSendTaskList(void)
{
    {
        SEGGER_SYSVIEW_TASKINFO Info = {
                .TaskID = TASKID_CYCLIC,
                .sName  = "TaskCyclic",
                .StackBase = 0x20004000,
                .StackSize = 0x2000,
                .Prio = 10,
        };
        SEGGER_SYSVIEW_OnTaskCreate(TASKID_CYCLIC);
        SEGGER_SYSVIEW_SendTaskInfo( &Info);
    }

    {
        SEGGER_SYSVIEW_TASKINFO Info = {
                .TaskID = TASKID_TESTFUNC0,
                .sName  = "_TestFunc0",
                .StackBase = 0x20006000,
                .StackSize = 0x4000,
                .Prio = 12,
        };
        SEGGER_SYSVIEW_OnTaskCreate(TASKID_TESTFUNC0);
        SEGGER_SYSVIEW_SendTaskInfo( &Info);
    }

    {
        SEGGER_SYSVIEW_TASKINFO Info = {
                .TaskID = TASKID_MAINLOOP,
                .sName  = "mainloop",
                .StackBase = 0x2000a000,
                .StackSize = 0x2000,
                .Prio = 4,
        };
        SEGGER_SYSVIEW_OnTaskCreate(TASKID_MAINLOOP);
        SEGGER_SYSVIEW_SendTaskInfo( &Info);
    }

    SEGGER_SYSVIEW_NameMarker(MARKER_PRINT, "MarkerPrint");
    SEGGER_SYSVIEW_NameMarker(MARKER_PRINT_MOD, "MarkerPrintModulo");

    // named resources are referenced in the description file via %I
    // or "NamedType" might be used (more flexibel)
    SEGGER_SYSVIEW_NameResource(10,  "10us");
    SEGGER_SYSVIEW_NameResource(100, "100us");
    SEGGER_SYSVIEW_NameResource(200, "0.2ms");
    SEGGER_SYSVIEW_NameResource(500, "0.5ms");
}   // _cbSendTaskList

static const SEGGER_SYSVIEW_OS_API _NoOSAPI = {NULL, _cbSendTaskList};



static void _Delay(uint32_t period_us)
{
    volatile int i;

    SEGGER_SYSVIEW_RecordU32(33, period_us);

#if 0
    if (period_us == 10)
        i = 45;
    else if (period_us == 100)
        i = 630;
    else if (period_us == 1000)
        i = 6400;
    else
#endif
        i = (6400 * period_us) / 1000;

    do {
        __asm volatile ("nop\n" : : :);
    } while (i--);
    SEGGER_SYSVIEW_RecordEndCall(33);
}   // _Delay



static void TaskCyclic(int i)
{
    SEGGER_SYSVIEW_OnTaskStartExec(TASKID_CYCLIC);

    //
    // small demo, that even small runtime differences are visible, task runtimes:
    // - debug variant: 70.5µs .. 101.8µs
    // - optimized:     35.8µs ..  37.1µs
    //
    for (int dd = 0;  dd < 20*i;  ++dd)
    {
        __asm volatile ("nop\n" : : :);
    }
    SEGGER_SYSVIEW_OnTaskStopReady(TASKID_CYCLIC, 0);
}   // TaskCyclic



typedef enum {
    STATE_INIT = 0,
    STATE_WAIT_FOR_DATA,
    STATE_PROCESS_DATA,
    STATE_SEND_DATA
} TASK_STATE_0;

static void _TestFunc0(void)
/**
 * test function taken from https://wiki.segger.com/Use_SystemView_without_RTOS
 */
{
    static TASK_STATE_0 _State = STATE_INIT;

    SEGGER_SYSVIEW_OnTaskStartExec(TASKID_TESTFUNC0);

    switch (_State) {
    case STATE_INIT:
        SEGGER_SYSVIEW_RecordVoid(50 + STATE_INIT);
        _Delay(20);
        _State = STATE_WAIT_FOR_DATA;
        SEGGER_SYSVIEW_RecordEndCall(50 + STATE_INIT);
        break;
    case STATE_WAIT_FOR_DATA:
        SEGGER_SYSVIEW_RecordVoid(50 + STATE_WAIT_FOR_DATA);
        _Delay(30);
        _State = STATE_PROCESS_DATA;
        SEGGER_SYSVIEW_RecordEndCall(50 + STATE_WAIT_FOR_DATA);
        break;
    case STATE_PROCESS_DATA:
        SEGGER_SYSVIEW_RecordVoid(50 + STATE_PROCESS_DATA);
        _Delay(40);
        _State = STATE_SEND_DATA;
        SEGGER_SYSVIEW_RecordEndCall(50 + STATE_PROCESS_DATA);
        break;
    case STATE_SEND_DATA:
        SEGGER_SYSVIEW_RecordVoid(50 + STATE_SEND_DATA);
        _Delay(50);
        _State = STATE_WAIT_FOR_DATA;
        SEGGER_SYSVIEW_RecordEndCall(50 + STATE_SEND_DATA);
        break;
    }
    SEGGER_SYSVIEW_OnTaskStopReady(TASKID_TESTFUNC0, 0);
}   // _TestFunc0



#define xFAST
#ifdef FAST
    // -> ~70000 events/s, 295 KByte/s
    #define SYSTICKS    20000
    #define IDLE_US     1000
    #define PRINT_MOD   1000
#else
    // -> ~1800 events/s, 10 KByte/s
    #define SYSTICKS    15
    #define IDLE_US     40000
    #define PRINT_MOD   20
#endif


int main()
{
    static uint8_t buffer[32768];    // just playing around...

    printf("012345678901234567123456789012345678901234567890123456789-----------------------------------\n");
#if 1
    for (int i = 0;  i < 1000;  ++i) {        // this delay is required to have a running CYCCNT after reset with pyocd
        _Delay(1000);
    }
#endif
    printf("012345678901234567123456789012345678901234567890123456789 - 1000\n");

    // initialize
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Init(SystemCoreClock, SystemCoreClock, &_NoOSAPI, _cbSendSystemDesc);
    SEGGER_RTT_ConfigUpBuffer(SEGGER_SYSVIEW_RTT_CHANNEL, "SysView", buffer, sizeof(buffer), SEGGER_RTT_MODE_NO_BLOCK_SKIP);

    SEGGER_SYSVIEW_Start();
    _Delay(10000);

    SysTick_Init(SYSTICKS);

    SEGGER_SYSVIEW_EnableEvents(0xffffffff);

#if 1
    for (int j = 0;  j < 5000000;  ++j) {
        SEGGER_SYSVIEW_OnTaskStartExec(TASKID_MAINLOOP);
        _Delay(200);
        SEGGER_SYSVIEW_PrintfTarget("Start %d\n", j);
        for (int i = 0;  i < 5;  ++i) {
            _TestFunc0();
            SEGGER_SYSVIEW_OnTaskStartExec(TASKID_MAINLOOP);
            SEGGER_SYSVIEW_MarkStart(MARKER_PRINT);
            SEGGER_SYSVIEW_Mark(MARKER_PRINT);
            TaskCyclic(i);
            SEGGER_SYSVIEW_OnTaskStartExec(TASKID_MAINLOOP);
            SEGGER_SYSVIEW_MarkStop(MARKER_PRINT);
            _Delay(0);
        }
        SEGGER_SYSVIEW_OnTaskStartExec(TASKID_MAINLOOP);
#if 1
        if (j != 0  &&  j % PRINT_MOD == 0) {
            SEGGER_SYSVIEW_MarkStart(MARKER_PRINT_MOD);
            printf("0123456789012345678901234567890123456789 %d\n", j);
            SEGGER_SYSVIEW_MarkStop(MARKER_PRINT_MOD);
        }
#endif
        _Delay(500);

        SEGGER_SYSVIEW_OnIdle();
        _Delay(IDLE_US);
    }

    SEGGER_SYSVIEW_DisableEvents(0xffffffff);
    SEGGER_SYSVIEW_Print("Stop\n");
    SEGGER_SYSVIEW_Stop();

    printf("finished\n");
#else
    for (;;) {
        SEGGER_SYSVIEW_IsStarted();

        _Delay(10);
        _Delay(100);
        _Delay(1000);
    }
#endif
}   // main
