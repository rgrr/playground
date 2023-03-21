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
#if 1
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

    for (int i = 0;  i < 97;  ++i) {
        _printf("-- %u\n", cnt++);
    }

#if 0
    static const uint8_t buf[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
#if 0
    FILE *f;
    size_t r;

    f = fopen("test.bin", "wb");
    _printf("f=%p\n", f);
    r = fwrite(buf, sizeof(buf), 1, f);
    _printf("r=%d\n", r);
    r = fclose(f);
    _printf("r=%d\n", r);
#else
    int f;
    int r;

    write(STDOUT_FILENO, "Aber Hallo\n", 11);

    f = open("llvm-games/profiling/test.binx", O_WRONLY | O_APPEND | O_CREAT, 0644);
    _printf("f=%d\n", f);
    r = write(f, buf, sizeof(buf));
    _printf("r=%d\n", r);
    r = close(f);
    _printf("r=%d\n", r);
#endif
#endif
#if 0
    {
        static char out[4096];
        r = __llvm_profile_write_buffer(out);
        _printf("r=%d\n", r);
    }
#endif
    __llvm_profile_write_file();
}   // main
