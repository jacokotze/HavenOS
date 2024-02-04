[global load_gdt]               ; Allows the C code to call load_gdt_c().

section .text
load_gdt:
   mov  eax, [esp+4] ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]        ; Load the new GDT pointer
   
   mov eax, 0x10 ; 0x10 is the offset in the GDT to our data segment
   mov ds,  eax  ; Load all data segment selectors
   mov es,  eax
   mov fs,  eax
   mov gs,  eax
   mov ss,  ax
   jmp 0x08:.flush
   ret
.flush:
    ret ; return to main; now it protected mode!

[global load_idt]               ; Allows the C code to call load_idt_c().

load_idt:
   mov  eax, [esp+4] ; Get the pointer to the IDT, passed as a parameter.
   lidt [eax]        ; Load the IDT pointer.
   ret