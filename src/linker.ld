ENTRY(kernel_main)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    . = 1M;
    KERNEL_START = .;
    .text : ALIGN(4k)
    {
        *(.text.kernel)
        *(.text)
    }

    .rodata : ALIGN(4k)
    {
        *(.rodata)
    }

    .data : ALIGN(4k)
    {
        *(.data)
    }

    .bss : ALIGN(4k)
    {
        *(COMMON)
        *(.bss)
    }

    .asm : ALIGN(4k)
    {
        *(.asm)
    }
    KERNEL_END = .;
}
