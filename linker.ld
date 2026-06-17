ENTRY(_start)

SECTIONS
{
    . = 0x1000;

    kernel_start = .;

    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }

    kernel_end = .;
}