/*
 * minimum linker script for clang
 */

MEMORY
{
    flash (rx!w) : ORIGIN = DEFINED(__flash) ? __flash : 0x00000000, LENGTH = DEFINED(__flash_size) ? __flash_size : 0x00100000
    ram (w!rx)   : ORIGIN = DEFINED(__ram  ) ? __ram   : 0x20000000, LENGTH = DEFINED(__ram_size  ) ? __ram_size   : 0x00040000
}


SECTIONS
{
    PROVIDE(__stack = ORIGIN(ram) + LENGTH(ram));

    .init : {
        KEEP (*(.text.init.enter))
    } >flash AT>flash :text

    .text : {
        . = ALIGN(4);
        *(.text .text.*)
    } >flash AT>flash

    .data :  {
        . = ALIGN(4);
        *(.data .data.*)
        PROVIDE(__data_end = .);
    } >ram AT>flash
    PROVIDE(__data_start = ADDR(.data));
    PROVIDE(__data_source = LOADADDR(.data));
    PROVIDE(__data_size = __data_end - __data_start);

    .bss (NOLOAD) : {
        . = ALIGN(4);
        *(.bss .bss.*)
        __bss_end__ = .;
    } >ram AT>ram
    PROVIDE( __bss_start__ = ADDR(.bss));
    PROVIDE( __bss_size__  = __bss_end__ - __bss_start__ );
    PROVIDE( __bss_start   = __bss_start);
    PROVIDE( __bss_size    = __bss_size__);

    .tdata :  {
        . = ALIGN(4);
        *(.tdata .tdata.*)
        __tdata_end = .;
    } >ram AT>flash
    PROVIDE( __tls_base = ADDR(.tdata));

    #PROVIDE( __tls_align = MAX(ALIGNOF(.tdata), ALIGNOF(.tbss)) );
    PROVIDE( __tls_align = ALIGNOF(.tdata) );
    PROVIDE( __arm32_tls_tcb_offset = MAX(8, __tls_align) );

    # my private symbol for end of RAM usage
    .mydata (NOLOAD) : {
        __my_data_end = .;
    } >ram AT>ram
}
