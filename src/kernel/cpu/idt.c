/*
Description: Allocates the physical memory for the IDT, formats the handler memory addresses to fit Intel's legacy structures, and loads the table into the CPU.
*/

#include "idt.h"
#include "./cpu_exceptions/isr.h"

// The x86 architecture physically supports exactly 256 hardware interrupts.
#define IDT_ENTRIES 256

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_ptr;

extern void idt_load(unsigned int);

// takes a standard 32-bit C memory address, chops it in half, and formats it into the 8-byte structure required by the x86 CPU hardware.
void idt_set_gate(unsigned char num, unsigned int base, unsigned short selector, unsigned char flags){
    idt[num].base_low = base & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
    idt[num].base_high = (base >> 16) & 0xFFFF;
}

void idt_init(){
    idt_ptr.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;

    idt_ptr.base = (unsigned int)&idt;

    idt_set_gate(0, (unsigned int)isr0, 0x08, 0x8E);
    idt_set_gate(1, (unsigned int)isr1, 0x08, 0x8E);
    idt_set_gate(2, (unsigned int)isr2, 0x08, 0x8E);
    idt_set_gate(3, (unsigned int)isr3, 0x08, 0x8E);
    idt_set_gate(4, (unsigned int)isr4, 0x08, 0x8E);
    idt_set_gate(5, (unsigned int)isr5, 0x08, 0x8E);
    idt_set_gate(6, (unsigned int)isr6, 0x08, 0x8E);
    idt_set_gate(7, (unsigned int)isr7, 0x08, 0x8E);
    idt_set_gate(8, (unsigned int)isr8, 0x08, 0x8E);
    idt_set_gate(9, (unsigned int)isr9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned int)isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned int)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned int)isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned int)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned int)isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned int)isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned int)isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned int)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned int)isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned int)isr19, 0x08, 0x8E);
    
    idt_load((unsigned int)&idt_ptr);
}