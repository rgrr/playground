/*
 * minimum linker script for clang and picolib
 *
 * WARNING: this is really not generic, but fits the needs of this simple test program
 *
 * For a generic picolib linker script check picolibc.ld
 */

MEMORY
{
    flash (rx!w) : ORIGIN = DEFINED(__flash) ? __flash : 0x00000000, LENGTH = DEFINED(__flash_size) ? __flash_size : 0x00100000
    ram (w!rx)   : ORIGIN = DEFINED(__ram  ) ? __ram   : 0x20000000, LENGTH = DEFINED(__ram_size  ) ? __ram_size   : 0x00040000
}


SECTIONS
{
    PROVIDE(__stack = ORIGIN(ram) + LENGTH(ram));

    #
    # interrupt vectors
    #
    .init : {
        KEEP (*(.text.init.enter))
    } >flash AT>flash

    #
    # code
    #
    .text : {
        *(.text .text.*)
    } >flash AT>flash

    #
    # uninitialized data (cleared by the startup code)
    # startup code requires aligned dword data and will otherwise fail
    #
    .bss (NOLOAD) : {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss .bss.*)
        . = ALIGN(4);
        __bss_end = .;
    } >ram AT>ram
    PROVIDE( __bss_size    = __bss_end - __bss_start);

    #
    # initialized data, copied in crt0.h
    #
    .data :  {
        . = ALIGN(4);
        __data_start = .;
        *(.data .data.*)
        . = ALIGN(4);
        __data_end = .;
    } >ram AT>flash
    PROVIDE(__data_source = LOADADDR(.data));
    PROVIDE(__data_size = __data_end - __data_start);

    #
    # thread local initialized data, copied in crt0.h
    # 
    .tdata :  {
        . = ALIGN(4);
        __tls_base = .;
        *(.tdata .tdata.*)
        . = ALIGN(4);
        __tdata_end = .;
    } >ram AT>flash
    PROVIDE( __tls_align = ALIGNOF(.tdata) );
    PROVIDE( __arm32_tls_tcb_offset = MAX(8, __tls_align) );

    #
    # my private symbol for end of RAM usage
    #
    .mydata (NOLOAD) : {
        __my_data_end = .;
    } >ram AT>ram
}
