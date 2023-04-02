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



__attribute__ ((format (printf, 1, 2))) void _printf(const char *format, ...)
 /**
  * With this printf helper semihosting output is much faster because then blocks and not characters
  * are written.
  * \note pyocd needs "2" as the file descriptor.
  */
{
#ifdef USE_SEMIHOSTING_LIB
    static char buf[128];
    va_list arglist;

    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf), format, arglist);
    va_end(arglist);
    write(STDOUT_FILENO, buf, strlen(buf));
#endif
}   // _printf



extern int __llvm_profile_write_buffer(char *Buffer);
extern int __llvm_profile_write_file(void);


int main()
{
    uint32_t cnt = 0;
    int r;

    for (int i = 0;  i < 61;  ++i) {
        _printf("-- %u\n", cnt++);
    }

#ifdef USE_SEMIHOSTING_LIB
    {
        static const uint8_t buf[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        int f;

        write(STDOUT_FILENO, "Aber Hallo\n", 11);

        f = open("test.bin", O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
        _printf("f=%d (open)\n", f);
        r = write(f, buf, sizeof(buf));
        _printf("r=%d (write)\n", r);
        r = close(f);
        _printf("r=%d (close)\n", r);
    }
#endif

#if 1
    {
        static char out[4096];
        r = __llvm_profile_write_buffer(out);
        _printf("r=%d (__llvm_profile_write_buffer)\n", r);
    }
#endif

    r = __llvm_profile_write_file();
    _printf("r=%d (__llvm_profile_write_file)\n", r);
}   // main
