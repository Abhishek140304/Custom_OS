/*
Description:
Initializes the Interrupt Descriptor Table (IDT), installs all CPU
exception handlers (ISR 0-31), and loads the IDT into the processor.
*/

#include "idt.h"
#include "../exceptions/isr.h"

#define IDT_ENTRIES 256

#define KERNEL_CS  0x08
#define IDT_FLAGS  0x8E

static idt_entry_t idt[IDT_ENTRIES];
static idt_ptr_t idt_ptr;

extern void idt_load(unsigned int);

/*
 * Populates a single IDT entry.
 *
 * num      -> IDT index
 * base     -> address of ISR/IRQ handler
 * selector -> GDT code segment selector
 * flags    -> gate flags
 */
void idt_set_gate(unsigned char num, unsigned int base, unsigned short selector, unsigned char flags){
    idt[num].base_low = base & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
    idt[num].base_high = (base >> 16) & 0xFFFF;
}


// Initializes the entire IDT.

void idt_init(){

    // IDT descriptor
    idt_ptr.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idt_ptr.base  = (unsigned int)&idt;

    // Clear all entries first.
    for(int i = 0; i < IDT_ENTRIES; i++){
        idt_set_gate(i, 0, 0, 0);
    }

    // CPU Exceptions (0-31)
    idt_set_gate(0, (unsigned int)isr0,  KERNEL_CS, IDT_FLAGS);
    idt_set_gate(1, (unsigned int)isr1,  KERNEL_CS, IDT_FLAGS);
    idt_set_gate(2, (unsigned int)isr2,  KERNEL_CS, IDT_FLAGS);
    idt_set_gate(3, (unsigned int)isr3,  KERNEL_CS, IDT_FLAGS);
    idt_set_gate(4, (unsigned int)isr4,  KERNEL_CS, IDT_FLAGS);
    idt_set_gate(5, (unsigned int)isr5,  KERNEL_CS, IDT_FLAGS);
    idt_set_gate(6, (unsigned int)isr6,  KERNEL_CS, IDT_FLAGS);
    idt_set_gate(7, (unsigned int)isr7,  KERNEL_CS, IDT_FLAGS);
    idt_set_gate(8, (unsigned int)isr8,  KERNEL_CS, IDT_FLAGS);
    idt_set_gate(9, (unsigned int)isr9,  KERNEL_CS, IDT_FLAGS);
    idt_set_gate(10, (unsigned int)isr10, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(11, (unsigned int)isr11, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(12, (unsigned int)isr12, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(13, (unsigned int)isr13, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(14, (unsigned int)isr14, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(15, (unsigned int)isr15, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(16, (unsigned int)isr16, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(17, (unsigned int)isr17, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(18, (unsigned int)isr18, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(19, (unsigned int)isr19, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(20, (unsigned int)isr20, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(21, (unsigned int)isr21, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(22, (unsigned int)isr22, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(23, (unsigned int)isr23, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(24, (unsigned int)isr24, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(25, (unsigned int)isr25, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(26, (unsigned int)isr26, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(27, (unsigned int)isr27, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(28, (unsigned int)isr28, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(29, (unsigned int)isr29, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(30, (unsigned int)isr30, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(31, (unsigned int)isr31, KERNEL_CS, IDT_FLAGS);

    // Load the IDT into the CPU's IDTR register.
    idt_load((unsigned int)&idt_ptr);
}