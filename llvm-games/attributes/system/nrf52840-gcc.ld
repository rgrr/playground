/*
 * minimum linker script for gcc
 */

MEMORY
{
    flash (rx!w) : ORIGIN = DEFINED(__flash) ? __flash : 0x00000000, LENGTH = DEFINED(__flash_size) ? __flash_size : 0x00100000
    ram (w!rx)   : ORIGIN = DEFINED(__ram  ) ? __ram   : 0x20000000, LENGTH = DEFINED(__ram_size  ) ? __ram_size   : 0x00040000
}


PHDRS
{
    text     PT_LOAD;
    ram_init PT_LOAD;
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

    .bss (NOLOAD) : {
        __bss_start = .;
        __bss_start__ = .;
        *(.bss .bss.*)
        . = ALIGN(4);
        __bss_end = .;
        __bss_end__ = .;
    } >ram AT>ram

    # my private symbol for end of RAM usage
    .mydata (NOLOAD) : {
        __my_data_end = .;
    } >ram AT>ram
}
