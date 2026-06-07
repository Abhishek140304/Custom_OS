; Description: Defines the Global Descriptor Table (GDT) required to enter 32-bit 
; Protected Mode. We define a "Flat Memory Model" by creating overlapping Code and Data 
; segments that both span from memory address 0x0 to 0xFFFFFFFF (4GB).
;
; ----------------------------------------------------------------------------------------

; null segment descriptor
gdt_start:
    dq 0x0


; code segment descriptor
gdt_code:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23

    ; 1st Flags Byte: (Present:1, Privilege:00, Descriptor Type:1) -> 1001
    ; Type Flags: (Code:1, Conforming:0, Readable:1, Accessed:0)   -> 1010
    db 10011010b 
    
    ; 2nd Flags Byte: (Granularity:1, 32-bit default:1, 64-bit:0, AVL:0) -> 1100
    ; Limit (bits 16-19) -> 1111
    db 11001111b

    db 0x0       ; segment base, bits 24-31



; data segment descriptor
gdt_data:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23


    ; 1st Flags Byte: (Present:1, Privilege:00, Descriptor Type:1) -> 1001
    ; Type Flags: (Code:0, Expand-down:0, Writable:1, Accessed:0)  -> 0010
    db 10010010b


    ; 2nd Flags Byte: (Granularity:1, 32-bit default:1, 64-bit:0, AVL:0) -> 1100
    ; Limit (bits 16-19) -> 1111
    db 11001111b

    db 0x0       ; segment base, bits 24-31

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit)
    dd gdt_start ; address (32 bit)

CODE_SEG equ gdt_code - gdt_start   ; Evaluates to 0x08
DATA_SEG equ gdt_data - gdt_start   ; Evaluates to 0x10