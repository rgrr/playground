/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>



// this results in "high" speed output on the gdb console with about "one line / 2s"

int main()
{
    uint32_t cnt = 0;

    while (true) {
        printf("-- %u\n", cnt++);
    }
}
