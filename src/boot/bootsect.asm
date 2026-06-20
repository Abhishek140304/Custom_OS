; Description: This is the primary 16-bit boot sector code. Its role is to 
; initialize basic CPU segments, set up a stack, load the compiled C kernel 
; from the disk into memory, capture the system memory map, and transition the 
; CPU from 16-bit Real Mode to 32-bit Protected Mode.
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

[BITS 16]   ; 16-bit "Real Mode"
[ORG 0x7c00]

KERNEL_OFFSET equ 0x1000 ; The memory address where we will load the Kernel

%define KERNEL_SECTORS 50   ; Number of sectors to load for kernel

MEMORY_MAP    equ 0x8000    ; place in RAM to store the memory map we get from the BIOS
MEMORY_COUNT  equ 0x7FF0

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

CALL load_kernel      ; Read the disk to put the C code at 0x1000
CALL detect_memory    ; Ask the BIOS exactly how much RAM the system has
CALL switch_to_32bit  ; Flip the CPU bit to enter Protected Mode!

JMP $   ; We never reach here

%include "./src/boot/disk.asm"
%include "./src/boot/gdt.asm"
%include "./src/boot/switch_to_32_bit.asm"

[BITS 16]
load_kernel:
    MOV bx, KERNEL_OFFSET   ; bx -> destination
    MOV dh, KERNEL_SECTORS  ; dh -> num sectors
    MOV dl, [BOOT_DRIVE]    ; dl -> disk
    CALL disk_load
    RET


[BITS 16]
detect_memory:
    mov di, MEMORY_MAP          ; Destination index: Where to write the array
    mov word [MEMORY_COUNT], 0
    xor ebx, ebx                ; EBX must be 0 to start the BIOS map loop
    
.e820_loop:
    mov eax, 0xE820             ; The BIOS function code for "Get Memory Map"
    mov edx, 0x534D4150         ; "SMAP" in ASCII. The BIOS requires this signature!    
    mov ecx, 24                 ; Request a 24-byte entry

    int 0x15                    ; BIOS Interrupt

    jc e820_failed

    cmp eax, 0x534D4150         ; The BIOS should return "SMAP" in EAX to confirm success.
    jne e820_failed

    inc word [MEMORY_COUNT]

    add di, 24

    test ebx, ebx
    jnz .e820_loop

    RET

e820_failed:
    jmp $


[BITS 32]
BEGIN_32BIT:
    ; 1. Copy the kernel from low memory (0x1000) to high memory (1 MB)
    mov esi, KERNEL_OFFSET      ; Source: 0x1000 (where the BIOS dumped it)
    mov edi, 0x100000           ; Destination: 1 MB mark

    ; Calculate how many DWORDs (4 bytes) to copy
    ; Since 1 sector is 512 bytes, and 1 DWORD is 4 bytes, there are exactly 128 DWORDs per sector.
    mov ecx, KERNEL_SECTORS * 128   ; Number of DWORDs to copy
    
    rep movsd                   ; Fast assembly instruction to copy ECX DWORDs from ESI to EDI

    ; 2. Execute the kernel
    CALL 0x100000               ; Jump to the new 1 MB location!

    jmp $                       ; Infinite loop safety net


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