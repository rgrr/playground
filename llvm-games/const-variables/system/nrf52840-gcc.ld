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
    #tls      PT_TLS;
}


SECTIONS
{
    PROVIDE(__stack = ORIGIN(ram) + LENGTH(ram));
    #PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));

    #
    # interrupt vectors
    #
    .init : {
        KEEP (*(.text.init.enter))
    } >flash AT>flash :text

    #
    # code
    #
    .text : {
        . = ALIGN(4);
        *(.text .text.*)
    } >flash AT>flash

    #
    # uninitialized data (cleared by the startup code)
    # startup code requires aligned dword data and will otherwise fail
    #
    .bss (NOLOAD) : {
        . = ALIGN(4);
        __bss_start = .;
        __bss_start__ = .;
        *(.bss .bss.*)
        . = ALIGN(4);
        __bss_end = .;
        __bss_end__ = .;
    } >ram AT>ram

    #
    # initialized data (copied by the startup assembler code)
    #
    __data_load_start = LOADADDR(.data);
    .data :
    {
        __data_start = .;
        *(.data .data.*)
        __data_end = .;
    } >ram AT>flash :ram_init

    #
    # my private symbol for end of RAM usage
    # "end" is required for sbrk in libnosys
    .mydata (NOLOAD) : {
        __my_data_end = .;
        end = .;
    } >ram AT>ram
}
