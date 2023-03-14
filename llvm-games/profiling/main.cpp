/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>



// this results in "high" speed output on the gdb console with about "one line / 2s"

extern "C" void __llvm_profile_write_buffer(uint8_t *);
uint8_t buff[4096];


int main()
{
    uint32_t cnt = 0;

    for (int i = 0;  i < 2;  ++i) {
        printf("-- %u\n", cnt++);
    }
    __llvm_profile_write_buffer(buff);
}
