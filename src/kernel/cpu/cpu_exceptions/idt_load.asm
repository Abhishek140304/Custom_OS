; Description: A tiny assembly routine exposed to our C kernel. It takes the memory 
; address of our IDT pointer (passed in from C) and loads it directly into the CPU's IDT 
; register.

[BITS 32]

global idt_load

idt_load:
    ; [esp]   = The return address back to the C code
    ; [esp+4] = The first argument we passed in (the pointer to our IDT)
    mov eax, [esp+4]
    lidt [eax]
    ret