/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdbool.h>
#include <SEGGER/RTT/SEGGER_RTT.h>


#define INC_CONST
#define INC_CONST_TINYUSB

#if defined(INC_CONST_TINYUSB)
    #include <string.h>
    #include <stdio.h>
#endif

//----------------------------------------------------------------------------------------------------------------------
//
// misc utilities
//
extern uint32_t __stack;
extern uint32_t __my_data_end;


static void _Delay_us(uint32_t period_us)
{
    volatile int i;

    i = 6400 * (period_us / 1000);
    do {
        __asm volatile ("nop\n" : : :);
    } while (i--);
}   // _Delay_us


static char const *p_eval( uint8_t const * const p )
{
    if (p >= (uint8_t *)&__my_data_end) {
        return "STACK";
    }
    else if (p >= (uint8_t *)0x20000000) {
        return "RAM";
    }
    return "FLASH";
}


//----------------------------------------------------------------------------------------------------------------------
//
// various const definitions and the corresponding memory layout
//
#ifdef INC_CONST

uint8_t u0[]              = "uint8_t []";
static uint8_t u1[]       = "static uint8_t *";
const uint8_t u2[]        = "const uint8_t *";
static const uint8_t u3[] = "static const uint8_t *";


static void p_u( uint8_t *p )
{
    SEGGER_RTT_printf(0, "  p_u('%s')=0x%08p - %s\n", (char *)p, p, p_eval(p));
}

static void constp_u( uint8_t * const p )
{
    SEGGER_RTT_printf(0, "  constp_u('%s')=0x%08p - %s\n", (char *)p, p, p_eval(p));
}

static void p_constu( uint8_t const *p )
{
    SEGGER_RTT_printf(0, "  p_constu('%s')=0x%08p - %s\n", (char *)p, p, p_eval(p));
}

static void constp_constu( uint8_t const * const p )
{
    SEGGER_RTT_printf(0, "  constp_constu('%s')=0x%08p - %s\n", (char *)p, p, p_eval(p));
}

static void print_const_memory_layout(void)
{
    _Delay_us(100 * 1000);
    p_u(u0);
    p_u(u1);

    _Delay_us(100 * 1000);
    constp_u(u0);
    constp_u(u1);

    _Delay_us(100 * 1000);
    p_constu(u0);
    p_constu(u1);
    p_constu(u2);
    p_constu(u3);

    _Delay_us(100 * 1000);
    constp_constu(u0);
    constp_constu(u1);
    constp_constu(u2);
    constp_constu(u3);
}   // print_const_memory_layout

#endif


//----------------------------------------------------------------------------------------------------------------------
//
// various function attributes
//
#if defined(INC_CONST_TINYUSB)

uint8_t tud_network_mac_address[6] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
const uint8_t const_tud_network_mac_address[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
static char abuf[253];   // check alignment ;-)

static const char *print_array_to_abuf(const void *buf, size_t len)
{
    const char *p = (const char *)buf;

    memset(abuf, 0, sizeof(abuf));
    for (size_t n = 0;  n < len;  ++n)
    {
        sprintf(abuf + 3*n, " %02x", p[n]);
    }
    return abuf;
}   // print_array_to_abuf


static void print_tinyusb_memory_layout(void)
{
    _Delay_us(100 * 1000);
    SEGGER_RTT_printf(0, "tud_network_mac_address=0x%08p - %s - %s\n",
                      tud_network_mac_address,
                      print_array_to_abuf(tud_network_mac_address, sizeof(tud_network_mac_address)),
                      p_eval(tud_network_mac_address));
    SEGGER_RTT_printf(0, "const_tud_network_mac_address=0x%08p - %s - %s\n",
                      const_tud_network_mac_address,
                      print_array_to_abuf(const_tud_network_mac_address, sizeof(const_tud_network_mac_address)),
                      p_eval(const_tud_network_mac_address));
}   // print_tinyusb_memory_layout

#endif


//----------------------------------------------------------------------------------------------------------------------


int main()
{
    SEGGER_RTT_printf(0, "\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    _Delay_us(1500 * 1000);
    SEGGER_RTT_printf(0, "Compiler:\n");
#if defined(__clang__)
    SEGGER_RTT_printf(0, "  clang %d.%d.%d (%s) -", __clang_major__, __clang_minor__, __clang_patchlevel__, __VERSION__);
#elif defined(__GNUC__)
    SEGGER_RTT_printf(0, "  gcc %d.%d.%d (%s) -", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__, __VERSION__);
#else
    SEGGER_RTT_printf(0, "  UNKNOWN - \n");
#endif
#if defined(__OPTIMIZE__)
    SEGGER_RTT_printf(0, " __OPTIMIZE__");
#endif
#if defined(__OPTIMIZE_SIZE__)
    SEGGER_RTT_printf(0, " __OPTIMIZE_SIZE__");
#endif
#if defined(__NO_INLINE__)
    SEGGER_RTT_printf(0, " __NO_INLINE__");
#endif
    SEGGER_RTT_printf(0, "\n");
    SEGGER_RTT_printf(0, "Compiled:\n  %s %s", __DATE__, __TIME__);
    SEGGER_RTT_printf(0, "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    SEGGER_RTT_printf(0, "\n========================================\n");
    SEGGER_RTT_printf(0, "Stack               : %p\n", &__stack);
    SEGGER_RTT_printf(0, "Data End            : %p\n", &__my_data_end);
    SEGGER_RTT_printf(0, "----------------------------------------\n");

#ifdef INC_CONST
    print_const_memory_layout();
    SEGGER_RTT_printf(0, "========================================\n");
#endif

#if defined(INC_CONST_TINYUSB)
    print_tinyusb_memory_layout();
    SEGGER_RTT_printf(0, "========================================\n");
#endif

    while (true) {
        _Delay_us(250 * 1000);
    }
}
