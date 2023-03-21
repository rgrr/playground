/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>
#include <pico/stdlib.h>
#ifdef CYW43_LWIP
    #include "pico/cyw43_arch.h"
#endif




static void led(uint8_t state)
{
#ifdef CYW43_LWIP
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
#elif defined(PICOPROBE_LED)
    static bool initialized;

    if ( !initialized) {
        initialized = true;
        gpio_init(PICOPROBE_LED);
        gpio_set_dir(PICOPROBE_LED, GPIO_OUT);
    }
    gpio_put(PICOPROBE_LED, state);
#endif
}   // led



int main()
{
    uint32_t cnt = 0;

    stdio_init_all();
    printf("Hello %s\n", PICO_TARGET_NAME);

#ifdef CYW43_LWIP
    if (cyw43_arch_init()) {
        printf("failed to initialize WiFi\n");
    }
#endif
    while (true) {
        printf("-- %u\n", cnt++);
        led(1);
        sleep_ms(250);
        led(0);
        sleep_ms(250);
    }
}
