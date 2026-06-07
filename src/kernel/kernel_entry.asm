; Description: This is the absolute entry point for our C kernel. It provides a safe 
; 32-bit environment, resolves the external C function, calls it, and provides a safety 
; net if the C code ever accidentally returns.

; ----------------------------------------------------------------------------------------


[BITS 32]

global _start
extern kernel_main

_start:
    call kernel_main

.hang:
    jmp .hang