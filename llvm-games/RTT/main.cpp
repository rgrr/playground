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



#if 0
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


#else
    /**
     * Uses stdout handler from picolic
     */

    #include <stdio.h>


    static int rtt_putc(char c, FILE *file)
    {
        (void) file;
        SEGGER_RTT_Write(0, &c, 1);
        return c;
    }


    static FILE __stdio = FDEV_SETUP_STREAM(rtt_putc, NULL, NULL, _FDEV_SETUP_WRITE);

    FILE *const stdout = &__stdio;


    int main(void)
    {
        uint32_t cnt = 0;

        do {
            printf("Hello, veryyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy long line %d\n", ++cnt);
            _Delay(50);
        } while (1);
        return 0;
    }


#endif
