[BITS 16]
[ORG 0x7c00]

KERNEL_OFFSET equ 0x1000 ; The memory address where we will load the Kernel

; Initialize segment registers
XOR ax, ax
MOV ds, ax
MOV es, ax
MOV ss, ax

; The BIOS secRETly stores our boot drive number in 'DL' when it starts.
; We must save it immediately to use it later!
MOV [BOOT_DRIVE], dl

; setup stack
MOV bp, 0x9000
MOV sp, bp

; Load the kernel and switch to 32-bit protected mode
CALL load_kernel
CALL switch_to_32bit

JMP $   ; We never reach here

%include "./src/boot/disk.asm"
%include "./src/boot/gdt.asm"
%include "./src/boot/switch_to_32_bit.asm"

[BITS 16]
load_kernel:
    MOV bx, KERNEL_OFFSET ; bx -> destination
    MOV dh, 20             ; dh -> num sectors
    MOV dl, [BOOT_DRIVE]  ; dl -> disk
    CALL disk_load
    RET

[BITS 32]
BEGIN_32BIT:
    CALL KERNEL_OFFSET ; give control to the kernel
    JMP $ ; loop in case kernel returns

; boot drive variable
BOOT_DRIVE DB 0

; padding
times 510 - ($-$$) db 0

; magic number
DW 0xaa55