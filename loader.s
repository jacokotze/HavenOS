KERNEL_STACK_SIZE equ 65535                 ; size of stack in bytes
MAGIC_NUMBER      equ 0x1BADB002            ; define the magic number constant
FLAGS             equ 3                     ; multiboot flags
CHECKSUM          equ -(MAGIC_NUMBER+FLAGS) ; calculate the checksum
                                ; (magic number + checksum + flags should equal 0)

BITS   32
[GLOBAL mboot]
[EXTERN code]
[EXTERN bss]
[EXTERN end]

section .__mbHeader
align   4           ; the code must be 4 byte aligned
    dd MAGIC_NUMBER ; write the magic number to the machine code,
    dd FLAGS        ; the flags,
    dd CHECKSUM     ; and the checksum
    ; BEGIN - Kernel stack space allocation
    Kernel_Stack_End:
        TIMES 65535 db 0
    Kernel_Stack_Start:
    ; END - Kernel stack space allocation

[global _Kernel_Entry] ; the entry symbol for ELF
[extern kmain]


_Kernel_Entry:
    mov  dword ECX, 0x2BADB002
	cmp  ECX,       EAX
    jne  (HandleNoMultiboot)
    cli   
    call kmain   ; goto
    ; mov  esp,       kernel_stack + KERNEL_STACK_SIZE
    jmp  $

HandleNoMultiboot:
	; BEGIN - Set Screen Colour
	mov  dword EAX,    0x4F    ; Colour: 0x4- = Red background, 0x-F = White foreground
	mov  dword EBX,    0xB8000 ; Display Memory address
	mov  dword ECX,    2000    ; 80 x 25 Characters - VGA Text-mode Display size
	.ColourOutput:
	mov  byte [EBX],   0
	mov  byte [EBX+1], AL
	add  EBX,          2
	loop .ColourOutput
	; END - Set Screen Colour

section .other_location
    [extern other_location_start]
    ; BEGIN - Set Screen Colour
	mov  dword EAX,    0x4F    ; Colour: 0x4- = Red background, 0x-F = White foreground
	mov  dword EBX,    0xB8000 ; Display Memory address
	mov  dword ECX,    2000    ; 80 x 25 Characters - VGA Text-mode Display size
	.ColourOutputA:
	mov  byte [EBX],   0
	mov  byte [EBX+1], AL
	add  EBX,          2
	loop .ColourOutputA
    jmp $

section       .bss
align         4    ; align at 4 bytes
kernel_stack:      ; label points to beginning of memory
    resb KERNEL_STACK_SIZE ; reserve stack for the kernel