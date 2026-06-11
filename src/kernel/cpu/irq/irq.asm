; Description: Assembly wrappers for Hardware Interrupt Requests (IRQs). This file 
; standardizes the stack and routes hardware signals to C-based 'irq_handler'.
; 
; ---------------------------------------------------------------------------------


[BITS 32]
extern irq_handler


%macro IRQ 2        ; Macro takes 2 arguments: the index (0-15) and the IDT vector (32-47)

global irq%1

irq%1:
    cli
    push dword 0
    push dword %2
    jmp irq_common

%endmacro



irq_common:
    pushad

    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
    add esp, 4

    pop eax

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popad

    add esp, 8

    sti
    iret



; The Intel 8259 PIC (Interrupt Controller) sends hardware signals on IRQs 0-15. We mapped 
; these to IDT vectors 32-47 so they don't collide with the 32 CPU exceptions (0-31).

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39

IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47