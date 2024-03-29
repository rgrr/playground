/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include "SEGGER/RTT/SEGGER_RTT.h"
#include <stdio.h>


static void _Delay(int period)
{
    volatile int i = (100000 / 17) * period;
    do {
        ;
    } while (i--);
}   // _Delay


/**
 * Uses stdio handler from picolic
 * See https://github.com/picolibc/picolibc/blob/main/doc/os.md
 */


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


int main(void)
{
    uint32_t cnt = 0;

    printf("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
    _Delay(32);

#if 0
    for (int i = 0;  i < 10;  ++i) {
#else
    for (;;) {
#endif
#if 0
        // output some UTF-8 characters
        printf("äöüÄÖÜßµ@€′¹²³¼½¬{[]}\\☻💋★ Helloho, veryyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy long line %d\r\n", ++cnt);
#else
        // output ASCII
        printf("@Helloho, veryyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy long line %d\n", ++cnt);
#endif
        _Delay(1517);
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
