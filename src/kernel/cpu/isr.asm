[BITS 32]

extern isr_handler

; macro for exceptions without error code

%macro ISR_NOERR 1

global isr%1

isr%1:
    cli 
    push dword 0
    push dword %1

    jmp isr_common

%endmacro



; macro with exceptions with error code
%macro ISR_ERR 1

global isr%1

isr%1:
    cli
    push dword %1

    jmp isr_common

%endmacro



; common handler
isr_common:
    pushad
    mov ax, ds  ; save segments
    push eax

    mov ax, 0x10    ; load kernel segments
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp

    call isr_handler

    add esp, 4  ; remove parameter

    pop eax ; restore segments

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popad

    add esp, 8  ; remove interrupt number and error code

    sti 
    iret




ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7

ISR_ERR   8

ISR_NOERR 9

ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14

ISR_NOERR 15
ISR_NOERR 16

ISR_ERR   17

ISR_NOERR 18
ISR_NOERR 19