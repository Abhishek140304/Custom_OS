; Description: This is the primary 16-bit boot sector code. Its role is to initialize
; basic CPU segments, set up a stack, load the compiled C kernel from the disk into memory
; (at 0x1000), and transition the CPU from 16-bit Real Mode into 32-bit Protected Mode.
;
; Complete Workflow:
; Step 1: BIOS loads bootloader:
;         BIOS loads first 512 bytes (boot sector) into memory at 0x7C00
;         Starts executing it in 16-bit real mode
; Step 2: Bootloader initializes environment
;         Sets segment registers
;         Sets up stack
;         Saves boot drive
; Step 3: Load kernel from disk
;         Reads sectors from disk (using BIOS INT 13h)
;         Loads kernel into memory at 0x1000
; Step 4: Switch to 32-bit protected mode
;         Load GDT
;         Enable protected mode
;         Far jump to flush pipeline
; Step 5: Setup 32-bit environment
;         Set segment registers
;         Setup stack
; Step 6: Jump to kernel
;         Call kernel at 0x1000
;
; ----------------------------------------------------------------------------------------

[BITS 16]
[ORG 0x7c00]

KERNEL_OFFSET equ 0x1000 ; The memory address where we will load the Kernel

; Initialize segment registers
XOR ax, ax
MOV ds, ax
MOV es, ax
MOV ss, ax

; The BIOS secretly stores our boot drive number in 'DL' when it starts.
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
    MOV dh, 20            ; dh -> num sectors
    MOV dl, [BOOT_DRIVE]  ; dl -> disk
    CALL disk_load
    RET

[BITS 32]
BEGIN_32BIT:
    CALL KERNEL_OFFSET ; Jump to the address where we loaded our C code (0x1000)

; boot drive variable
BOOT_DRIVE DB 0

; Padding: A valid boot sector MUST be exactly 512 bytes.
; This calculates how many bytes we've used ($-$$) and pads the rest with zeros.
times 510 - ($-$$) db 0

; The Magic Signature (0xAA55): The BIOS requires proof that the 512-byte sector it just
; read is actually meant to be booted. It checks the very last two bytes (bytes 511 and
; 512). If they aren't exactly 0xAA55, the BIOS will throw a "No bootable device found" 
; error and halt.
DW 0xaa55