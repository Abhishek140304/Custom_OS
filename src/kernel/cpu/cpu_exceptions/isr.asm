; Description: Assembly wrappers for CPU Interrupt Service Routines. This file 
; standardizes the stack state for different types of exceptions before calling the 
; C-based 'isr_handler'.
;
;----------------------------------------------------------------------------------


[BITS 32]
extern isr_handler

; 1. The Macros: Standardizing the Stack
; Intel architecture is messy: some exceptions push an error code to the stack 
; automatically, and some do not. We use these macros to ensure that by the time we hit 
; 'isr_common', the stack looks exactly the same.


; macro with exceptions without error code
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
    mov ax, ds      ; save segments
    push eax

    mov ax, 0x10    ; load kernel data segment (GDT index 0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp        ; Push the stack pointer: THIS IS THE ARGUMENT to isr_handler! It lets 
                    ; the C code see the entire CPU snapshot.

    call isr_handler

    add esp, 4      ; remove parameter

    pop eax         ; restore segments

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popad

    add esp, 8      ; remove interrupt number and error code

    sti             ; Re-enable interrupts
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
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29

ISR_ERR   30

ISR_NOERR 31

