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



// this results in "high" speed output on the gdb console with about "one line / 2s"

__attribute__ ((format (printf, 1, 2))) void _printf(const char *format, ...)
 /**
  * With this printf helper openocd semihosting output is much faster because then blocks and not characters
  * are written.
  *
  * BUT: there is no output with pyocd
  */
{
    static char buf[128];
    va_list arglist;

    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf), format, arglist);
    va_end(arglist);
    write(STDOUT_FILENO, buf, strlen(buf));
}   // _printf



int main()
{
    uint32_t cnt = 0;

    while (true) {
#if 1
        printf("-- %u\n", cnt++);
#else
        _printf("-- %u\n", cnt++);
#endif
    }
}
