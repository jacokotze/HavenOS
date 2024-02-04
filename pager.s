[global asm_enable_paging]
[global asm_load_page_directory]
section .text
asm_load_page_directory:
    ; eax has the address of the page directory
    mov eax, [esp+4]    ; get pointer from function call
    mov cr3, eax

    mov ebx, cr4        ; read current cr4
    or  ebx, 0x00000010 ; set PSE
    mov cr4, ebx        ; update cr4
ret

asm_enable_paging:
    mov ebx, cr0        ; read current cr0
    or  ebx, 0x80000000 ; set PG
    mov cr0, ebx        ; update cr0
    ; now paging is enabled
ret


section .text
global loadPageDirectory
loadPageDirectory:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]
    mov cr3, eax
    mov esp, ebp
    pop ebp
ret

section .text
global enablePaging
enablePaging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov esp, ebp
    pop ebp
ret