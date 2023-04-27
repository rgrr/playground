/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdio.h>


static void _Delay(int period)
{
    volatile int i = (100000 / 17) * period;
    do {
        ;
    } while (i--);
}   // _Delay


/**
 * Uses librtt.a from toolchain
 */
int main(void)
{
    uint32_t cnt = 0;

    printf("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------\n");
    _Delay(32);

    for (;;) {
        // output ASCII
        printf("@Helloho, veryyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy long line with librtt %d\n", ++cnt);
        _Delay(817);
        if (cnt % 8 == 0)
        {
            _Delay(235);
        }

        for (;;)
        {
            int ch;
            ch = getchar();
            if (ch == EOF)
            {
                break;
            }
            printf("received char: 0x%04x %c\n", ch, ch);
        }
    }
    printf("finished\n");
    return 0;
}
