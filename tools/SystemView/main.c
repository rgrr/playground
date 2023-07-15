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

#include "SEGGER_SYSVIEW.h"



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
#include "SEGGER_RTT.h"

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
 *********************************************************************/

#define SYSVIEW_APP_NAME    "SysView Games"
#define SYSVIEW_DEVICE_NAME "PCA10056 nRF52840"
#define SYSTEM_CORE_NAME    "Cortex-M4"

#define MARKER_PRINT   0x2222
#define TASKID_PRINT   0x22
#define TASKID_DELAY   0x11
#define EV_ID          (IPModule.EventOffset + 0) // event ids must be >= 32


static void _cbSendIPModuleDesc(void);

SEGGER_SYSVIEW_MODULE IPModule = {
    "M=TestSysView,"                       \
    "0 XXXXXXXX cnt=%d",                             // sModule
    1,                                            // NumEvents
    0,                                            // EventOffset, Set by SEGGER_SYSVIEW_RegisterModule()
    _cbSendIPModuleDesc,                          // pfSendModuleDesc, NULL: No additional module description
    NULL,                                         // pNext, Set by SEGGER_SYSVIEW_RegisterModule()
};

static void _cbSendIPModuleDesc(void)
{
    SEGGER_SYSVIEW_NameResource(0x99991111, "Rx FIFO");
    SEGGER_SYSVIEW_NameResource(0x11119999, "Tx FIFO");
    SEGGER_SYSVIEW_RecordModuleDescription(&IPModule, "T=IP, S='embOS/IP V12.09'");
}


static void _Delay(int period)
{
    SEGGER_SYSVIEW_OnTaskStartExec(TASKID_DELAY);
    volatile int i = (600000 / (17*6+2)) * period + 100;
    do {
    } while (i--);
    SEGGER_SYSVIEW_OnTaskStopExec();
}   // _Delay


static void _cbSendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=" SYSVIEW_APP_NAME ",D=" SYSVIEW_DEVICE_NAME ",C=" SYSTEM_CORE_NAME ",O=NoOS");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");

//    SEGGER_SYSVIEW_RegisterModule( &IPModule );
    printf("aaaaa %lu\n", IPModule.EventOffset);

    //
    // name "tasks"
    //
//    _Delay(5);
    {
        SEGGER_SYSVIEW_TASKINFO Info;

        SEGGER_SYSVIEW_OnTaskCreate(TASKID_PRINT);
        memset( &Info, 0, sizeof(Info));
        Info.TaskID = TASKID_PRINT;
        Info.sName = "PrintCycCnt";
        SEGGER_SYSVIEW_SendTaskInfo( &Info);
    }

//    _Delay(5);
    {
        SEGGER_SYSVIEW_TASKINFO Info;

        SEGGER_SYSVIEW_OnTaskCreate(TASKID_DELAY);
        memset( &Info, 0, sizeof(Info));
        Info.TaskID = TASKID_DELAY;
        Info.sName = "Delay";
        SEGGER_SYSVIEW_SendTaskInfo( &Info);
    }

//    _Delay(5);
    SEGGER_SYSVIEW_NameMarker(MARKER_PRINT, "MarkerPrint");
}   // _cbSendSystemDesc


void SEGGER_SYSVIEW_Conf(void)
{
    static uint8_t buffer[32768];    // just playing around...
    CycCnt_Init();

    SEGGER_SYSVIEW_Init(SystemCoreClock, SystemCoreClock, NULL, _cbSendSystemDesc);
    SEGGER_RTT_ConfigUpBuffer(SEGGER_SYSVIEW_RTT_CHANNEL, "SysView", buffer, sizeof(buffer), SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    //SEGGER_SYSVIEW_SetRAMBase(0x20000000);    // effect??

//    SEGGER_SYSVIEW_RegisterModule( &IPModule );
    printf("xxxxx %lu\n", IPModule.EventOffset);
}   // SEGGER_SYSVIEW_Conf



static void PrintCycCnt(int i)
{
//    SEGGER_SYSVIEW_RecordU32(EV_ID, i);
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
    SEGGER_SYSVIEW_OnTaskStopExec();
//    SEGGER_SYSVIEW_RecordEndCallU32(EV_ID, (U32)DWT_CYCCNT());
}   // PrintCycCnt



int main()
{
    printf("012345678901234567123456789012345678901234567890123456789\n");

#if 1
    for (int i = 0;  i < 200;  ++i) {        // this delay is required to have a running CYCCNT after reset with pyocd
        _Delay(10);
    }
#endif

    printf("012345678901234567123456789012345678901234567890123456789\n");

    SEGGER_SYSVIEW_Conf();
//    SEGGER_SYSVIEW_Start();
    _Delay(10);

    SysTick_Init(30);      // 25000 and _Delay(2) below works with NCM (SWD freq=10M)

    SEGGER_SYSVIEW_EnableEvents(0xffffffff);

    for (int j = 0;  j < 5000000;  ++j) {
        SEGGER_SYSVIEW_PrintfTarget("Start %d\n", j);
        SEGGER_SYSVIEW_OnIdle();
        for (int i = 0;  i < 5;  ++i) {
            SEGGER_SYSVIEW_MarkStart(MARKER_PRINT);
            SEGGER_SYSVIEW_OnIdle();
            SEGGER_SYSVIEW_Mark(MARKER_PRINT);
            PrintCycCnt(i);
            SEGGER_SYSVIEW_OnIdle();
            SEGGER_SYSVIEW_MarkStop(MARKER_PRINT);
            SEGGER_SYSVIEW_OnIdle();
            _Delay(0);
        }
#if 1
        if (j % 1000 == 0)
            printf("0123456789012345678901234567890123456789 %d\n", j);
#endif
        _Delay(20);        // 2 & 3 works as well
    }

    SEGGER_SYSVIEW_DisableEvents(0xffffffff);
    SEGGER_SYSVIEW_Print("Stop\n");
    SEGGER_SYSVIEW_Stop();

    printf("finished\n");

    for (;;) {
        //SEGGER_SYSVIEW_IsStarted();
    }
}   // main
