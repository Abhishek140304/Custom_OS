/*
Description: Defines the strict memory structures required by the x86 CPU to route hardware interrupts and software exceptions to our C-based handler functions.
*/

#ifndef IDT_H
#define IDT_H

// 64 bit IDT Gate
typedef struct{
    // The lower 16 bits of the handler function's memory address
    unsigned short base_low;

    // The kernel segment selector (points to Code Segment in the GDT)
    unsigned short selector;

    // This MUST always be zero. The hardware requires it.
    unsigned char zero;

    // Flags (8 bits):
    // Bit 7: "Interrupt is present" (1)
    // Bits 6-5: Privilege level of caller (0=Kernel, 3=User)
    // Bit 4: Set to 0 for interrupt gates
    // Bits 3-0: Gate Type (14 = 32-bit Interrupt Gate)
    unsigned char flags;

    // The upper 16 bits of the handler function's memory address
    unsigned short base_high;
} __attribute__((packed)) idt_entry_t;


typedef struct{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_ptr_t;


// Initializes the 256 IDT entries and loads them into the CPU.
void idt_init();

#endif