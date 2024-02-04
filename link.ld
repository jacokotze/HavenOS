ENTRY(_Kernel_Entry)                /* the name of the entry label */

SECTIONS {

    .__mbHeader 0x100000 : 
    {
      *(.__mbHeader)
    }

    .text :   /* align at 4 KB */
    {
        code = .; _code = .; __code = .;
        *(.text)             /* all text sections from all files */
        *(.rodata*)          /* all read-only data sections from all files */
        . = ALIGN(4096);
    }

    .data ALIGN (0x1000) :   /* align at 4 KB */
    {
        data = .; _data = .; __data = .;
        *(.data)             /* all data sections from all files */
        . = ALIGN(4096);

    }

    .bss ALIGN (0x1000) :    /* align at 4 KB */
    {
        bss = .; _bss = .; __bss = .;
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
        . = ALIGN(4096);

    }

    end = .; _end = .; __end = .;
    other_location = .; _other_location = .; __other_location = .;
}