global loader                   ; the entry symbol for ELF
    KERNEL_STACK_SIZE equ 4096                  ; size of stack in bytes
    MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
    FLAGS        equ 0x0            ; multiboot flags
    CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
                                    ; (magic number + checksum + flags should equal 0)

    section .text:                  ; start of the text (code) section
    align 4                         ; the code must be 4 byte aligned
        dd MAGIC_NUMBER             ; write the magic number to the machine code,
        dd FLAGS                    ; the flags,
        dd CHECKSUM                 ; and the checksum
    
    

    loader:                         ; the loader label (defined as entry point in linker script)
        mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the

    [bits 32]
        ; attempting to enter 32 bit protected mode
        cli
        lgdt  [gdtr]
        mov   eax, cr0
        or    al, 1
        mov   cr0, eax
        jmp   08h:kernel_main

        gdt_start:
        gdtr:
            dw gdt_end - gdt_start - 1
            dd gdt_start
            dw 0
        gdt_code:
            dw 0xFFFF
            dw 0
            db 0
            db 10011010b
            db 11001111b
            db 0
        gdt_data:
            dw 0xFFFF
            dw 0
            db 0
            db 10010010b
            db 11001111b
            db 0
        gdt_end:

        bits 32
        kernel_main:
            ; main kernel code
            mov   eax, 0x10
            mov   ds, eax 
            mov   es, eax
            mov   fs, eax
            mov   gs, eax
            mov   ss, ax
            movzx esp, sp

            ; Display hi in grey at top of screen
            mov   dword [0xB8000], 0x07690748
            extern main
            call main                    ; JUMP to main() in kmain.c

section .bss
align 4                                     ; align at 4 bytes
kernel_stack:                               ; label points to beginning of memory
    resb KERNEL_STACK_SIZE                  ; reserve stack for the kernel
                                            ; stack (end of memory area)