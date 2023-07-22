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

#include "SEGGER/RTT/SEGGER_SYSVIEW.h"



/*********************************************************************
 *
 *    Direct access to MCU registers
 *
 *********************************************************************/

#define DWT_CYCCNT()              (DWT->CYCCNT)


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


void CycCnt_Init(void)
{
    //
    // If no debugger is connected, the DWT must be enabled by the application
    //
    if ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) == 0) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    }

    //
    //  The cycle counter must be activated in order
    //  to use time related functions.
    //
    if ((DWT->CTRL & DWT_CTRL_NOCYCCNT_Msk) == 0) {           // Cycle counter supported?
        if ((DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) == 0) {      // Cycle counter not enabled?
            DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;              // Enable Cycle counter
        }
    }
}   // CycCnt_Init


//======================================================================================================================
#include "SEGGER/RTT/SEGGER_RTT.h"

static int rtt_putc(char c, FILE *file)
{
    (void) file;
    SEGGER_RTT_Write(0, &c, 1);
    return c;
}   // rtt_putc

static int rtt_getc(FILE *file)
{
    (void) file;
    uint8_t buf;
    unsigned r = SEGGER_RTT_Read(0, &buf, sizeof(buf));
    return (r == 0) ? EOF : buf;
}   // rtt_getc

static FILE __stdio = FDEV_SETUP_STREAM(rtt_putc, rtt_getc, NULL, _FDEV_SETUP_RW);

//FILE *const stdout = &__stdio;
FILE *const stdin = &__stdio;
__strong_reference(stdin, stdout);
__strong_reference(stdin, stderr);
//======================================================================================================================



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
#define TASKID_PRINT        0x22
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
                .TaskID = TASKID_PRINT,
                .sName  = "PrintCycCnt",
                .StackBase = 0x20004000,
                .StackSize = 0x2000,
                .Prio = 10,
        };
        SEGGER_SYSVIEW_OnTaskCreate(TASKID_PRINT);
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


void SEGGER_SYSVIEW_Conf(void)
{
    static uint8_t buffer[32768];    // just playing around...
    CycCnt_Init();

    SEGGER_SYSVIEW_Init(SystemCoreClock, SystemCoreClock, &_NoOSAPI, _cbSendSystemDesc);
    SEGGER_RTT_ConfigUpBuffer(SEGGER_SYSVIEW_RTT_CHANNEL, "SysView", buffer, sizeof(buffer), SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    //SEGGER_SYSVIEW_SetRAMBase(0x20000000);    // effect??
}   // SEGGER_SYSVIEW_Conf



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



static void PrintCycCnt(int i)
{
    SEGGER_SYSVIEW_OnTaskStartExec(TASKID_PRINT);

    //
    // small demo, that even small runtime differences are visible, task runtimes:
    // - debug variant: 70.5µs .. 101.8µs
    // - optimized:     35.8µs ..  37.1µs
    //
    for (int dd = 0;  dd < 20*i;  ++dd)
    {
        __asm volatile ("nop\n" : : :);
    }
    SEGGER_SYSVIEW_OnTaskStopReady(TASKID_PRINT, 0);
}   // PrintCycCnt



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
    printf("012345678901234567123456789012345678901234567890123456789-----------------------------------\n");
#if 1
    for (int i = 0;  i < 1000;  ++i) {        // this delay is required to have a running CYCCNT after reset with pyocd
        _Delay(1000);
    }
#endif
    printf("012345678901234567123456789012345678901234567890123456789 - 1000\n");

    SEGGER_SYSVIEW_Conf();

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
            PrintCycCnt(i);
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
