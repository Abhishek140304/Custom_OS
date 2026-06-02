[BITS 16]

; Loads 'dh' sectors to ES:BX from drive 'dl'
disk_load:
    PUSHA
    PUSH dx

    MOV ah, 0x02 ; read mode
    MOV al, dh   ; read dh number of sectors
    MOV cl, 0x02 ; start from sector 2
                 ; (as sector 1 is our boot sector)
    MOV ch, 0x00 ; cylinder 0
    MOV dh, 0x00 ; head 0

    ; dl = drive number is set as input to disk_load
    ; es:bx = buffer pointer is set as input as well

    INT 0x13      ; BIOS interrupt to read the disk
    JC disk_error ; Jump if the Carry Flag is set (BIOS sets this on error)

    POP dx     ; get back original number of sectors to read
    CMP al, dh ; BIOS sets 'al' to the # of sectors actually read compare it to 'dh' and error out if they are not equal
    JNE sectors_error
    POPA
    RET

disk_error:
    JMP disk_loop

sectors_error:
    JMP disk_loop

disk_loop:
    JMP $