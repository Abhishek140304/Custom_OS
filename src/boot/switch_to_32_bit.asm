; Description: This routine safely transitions the CPU from 16-bit Real Mode into 32-bit 
; Protected Mode. It disables interrupts, loads the Global Descriptor Table (GDT), flips 
; the hardware switch in cr0, flushes the CPU pipeline, and sets up a new 32-bit 
; environment.
;
; ----------------------------------------------------------------------------------------

[BITS 16]
switch_to_32bit:
    CLI                     ; 1. disable interrupts
    LGDT [gdt_descriptor]   ; 2. load GDT descriptor
    MOV eax, cr0
    or eax, 0x1             ; 3. enable protected mode
    MOV cr0, eax
    jmp CODE_SEG:init_32bit ; 4. far jump

[BITS 32]
init_32bit:
    MOV ax, DATA_SEG        ; 5. update segment registers
    MOV ds, ax
    MOV ss, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax

    MOV ebp, 0x90000        ; 6. setup stack
    MOV esp, ebp

    CALL BEGIN_32BIT        ; 7. move back to mbr.asm