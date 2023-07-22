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



/**
 *
 */

typedef struct {
    uint32_t    id;
    const char *name;
} SYSVIEW_INFO;

#define NUM_TASKS    32
static SYSVIEW_INFO  _aTasks[NUM_TASKS];
static int           _NumTasks;

#define NUM_MARKER   16
static SYSVIEW_INFO  _aMarker[NUM_MARKER];
static int           _NumMarker;

#define NUM_INTS     16
static SYSVIEW_INFO  _aInts[NUM_INTS];
static int           _NumInts;

static const char  *sysviewAppName;
static const char  *sysviewDeviceName;
static const char  *sysviewCoreName;
static const char  *sysviewOsName;



void SYSVIEW_AddTask(uint32_t taskId, const char *name)
{
    int n;

    SEGGER_SYSVIEW_OnTaskCreate(taskId);

    if (_NumTasks > NUM_TASKS) {
        return;
    }
    n = _NumTasks;
    _NumTasks++;

    _aTasks[n].id   = taskId;
    _aTasks[n].name = name;
}   // SYSVIEW_AddTask


void SYSVIEW_AddMarker(uint32_t markerId, const char *name)
{
    int n;

    if (_NumMarker > NUM_MARKER) {
        return;
    }
    n = _NumMarker;
    _NumMarker++;

    _aMarker[n].id   = markerId;
    _aMarker[n].name = name;
}   // SYSVIEW_AddMarker


void SYSVIEW_AddInt(uint32_t intId, const char *name)
{
    int n;

    if (_NumInts > NUM_INTS) {
        return;
    }
    n = _NumInts;
    _NumInts++;

    _aInts[n].id   = intId;
    _aInts[n].name = name;
}   // SYSVIEW_AddInt


void SYSVIEW_SetSystem(const char *appName, const char *deviceName, const char *coreName, const char *osName)
/**
 * Function descriptions go to /opt/SEGGER/SystemView_V<version>/Description/SYSVIEW_<osName>.txt
 */
{
    sysviewAppName    = appName;
    sysviewDeviceName = deviceName;
    sysviewCoreName   = coreName;
    sysviewOsName     = osName;
}   // SYSVIEW_SetSystem


static void _cbSendSystemDesc(void)
{
    char buf[128];

    if (sysviewAppName != NULL) {
        sprintf(buf, "N=%s", sysviewAppName);
        SEGGER_SYSVIEW_SendSysDesc(buf);
    }
    if (sysviewDeviceName != NULL) {
        sprintf(buf, "D=%s", sysviewDeviceName);
        SEGGER_SYSVIEW_SendSysDesc(buf);
    }
    if (sysviewCoreName != NULL) {
        sprintf(buf, "C=%s", sysviewCoreName);
        SEGGER_SYSVIEW_SendSysDesc(buf);
    }
    if (sysviewOsName != NULL) {
        sprintf(buf, "O=%s", sysviewOsName);
        SEGGER_SYSVIEW_SendSysDesc(buf);
    }

    for (int n = 0; n < _NumInts; n++) {
        sprintf(buf, "I#%d=%s", _aInts[n].id, _aInts[n].name);
        SEGGER_SYSVIEW_SendSysDesc(buf);
    }
}   // _cbSendSystemDesc


static void _cbSendTaskList(void)
{
    for (int n = 0; n < _NumTasks; n++) {
        SEGGER_SYSVIEW_TASKINFO info;

        memset(&info, 0, sizeof(info));
        info.TaskID = _aTasks[n].id;
        info.sName  = _aTasks[n].name;
        SEGGER_SYSVIEW_SendTaskInfo(&info);
    }
    for (int n = 0; n < _NumMarker; n++) {
        SEGGER_SYSVIEW_NameMarker(_aMarker[n].id, _aMarker[n].name);
    }
}   // _cbSendTaskList


void SYSVIEW_Init(uint32_t sysFreq, uint32_t cpuFreq, uint64_t (*getTime)(void))
{
    static SEGGER_SYSVIEW_OS_API osAPI = {NULL, _cbSendTaskList};

    osAPI.pfGetTime = getTime;
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Init(sysFreq, cpuFreq, &osAPI, _cbSendSystemDesc);
}   // SYSVIEW_Init


void SYSVIEW_ConfigUpBuffer(void* pBuffer, unsigned BufferSize)
{
    SEGGER_RTT_ConfigUpBuffer(SEGGER_SYSVIEW_RTT_CHANNEL, "SysView", pBuffer, BufferSize, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}   // SYSVIEW_ConfigUpBuffer


/**
 *
 */


void SysTick_Handler(void)
{
    static int cnt;

    SEGGER_SYSVIEW_RecordEnterISR();
    if (++cnt % 2 == 0) {
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
        __asm volatile ("nop\n" : : :);
    }
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

#define MARKER_PRINT        1
#define MARKER_PRINT_MOD    3
#define TASKID_CYCLIC       0x22
#define TASKID_TESTFUNC0    0x99
#define TASKID_MAINLOOP     0x1234
#define FUNCID_DELAY        33                     // must be >= 32
#define FUNCID_TESTBASE     50


static void _Delay(uint32_t period_us)
{
    volatile int i;

    SEGGER_SYSVIEW_RecordU32(FUNCID_DELAY, period_us);

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
    SEGGER_SYSVIEW_RecordEndCall(FUNCID_DELAY);
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
        SEGGER_SYSVIEW_RecordVoid(FUNCID_TESTBASE + STATE_INIT);
        _Delay(20);
        _State = STATE_WAIT_FOR_DATA;
        SEGGER_SYSVIEW_RecordEndCall(FUNCID_TESTBASE + STATE_INIT);
        break;
    case STATE_WAIT_FOR_DATA:
        SEGGER_SYSVIEW_RecordVoid(FUNCID_TESTBASE + STATE_WAIT_FOR_DATA);
        _Delay(30);
        _State = STATE_PROCESS_DATA;
        SEGGER_SYSVIEW_RecordEndCall(FUNCID_TESTBASE + STATE_WAIT_FOR_DATA);
        break;
    case STATE_PROCESS_DATA:
        SEGGER_SYSVIEW_RecordVoid(FUNCID_TESTBASE + STATE_PROCESS_DATA);
        _Delay(40);
        _State = STATE_SEND_DATA;
        SEGGER_SYSVIEW_RecordEndCall(FUNCID_TESTBASE + STATE_PROCESS_DATA);
        break;
    case STATE_SEND_DATA:
        SEGGER_SYSVIEW_RecordVoid(FUNCID_TESTBASE + STATE_SEND_DATA);
        _Delay(50);
        _State = STATE_WAIT_FOR_DATA;
        SEGGER_SYSVIEW_RecordEndCall(FUNCID_TESTBASE + STATE_SEND_DATA);
        break;
    }
    SEGGER_SYSVIEW_OnTaskStopReady(TASKID_TESTFUNC0, 0);
}   // _TestFunc0



#define xFAST
#ifdef FAST
    // -> ~70000 events/s, 295 KByte/s
    #define SYSTICKS_PER_SEC    20000
    #define IDLE_US             1000
    #define PRINT_MOD           1000
#else
    // -> ~1800 events/s, 10 KByte/s
    #define SYSTICKS_PER_SEC    15
    #define IDLE_US             40000
    #define PRINT_MOD           20
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
    SYSVIEW_Init(SystemCoreClock, SystemCoreClock, NULL);
    SYSVIEW_ConfigUpBuffer(buffer, sizeof(buffer));

    SYSVIEW_SetSystem("SysView Games", "PCA10056 nRF52840", "Cortex-M4", "NoOS");
    SYSVIEW_AddMarker(MARKER_PRINT,     "MarkerPrint");
    SYSVIEW_AddMarker(MARKER_PRINT_MOD, "MarkerPrintModulo");
    SYSVIEW_AddTask(TASKID_CYCLIC,    "Cyclic");
    SYSVIEW_AddTask(TASKID_MAINLOOP,  "Main");
    SYSVIEW_AddTask(TASKID_TESTFUNC0, "TestFunc0");
    SYSVIEW_AddInt(15, "SysTick");

    //SEGGER_SYSVIEW_Start();
    _Delay(10000);

    SysTick_Init(SYSTICKS_PER_SEC);

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
