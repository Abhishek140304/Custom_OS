/*
Description:
Initializes hardware interrupts (IRQ0-IRQ15) by installing the
assembly IRQ wrappers into the IDT. It also provides the master
IRQ dispatcher that sends End-Of-Interrupt (EOI) commands to the
PIC and routes interrupts to registered handlers.
*/

#include "irq.h"
#include "../idt/idt.h"
#include "../io/ports.h"
#include "../interrupt_manager/interrupt_manager.h"

#define KERNEL_CS  0x08
#define IDT_FLAGS  0x8E


// IRQ Assembly Wrappers

extern void irq0();      // Timer
extern void irq1();      // Keyboard
extern void irq2();      // Cascade
extern void irq3();      // COM2
extern void irq4();      // COM1
extern void irq5();      // LPT2
extern void irq6();      // Floppy Disk
extern void irq7();      // LPT1
extern void irq8();      // RTC
extern void irq9();      // General Purpose
extern void irq10();     // General Purpose
extern void irq11();     // General Purpose
extern void irq12();     // PS/2 Mouse
extern void irq13();     // FPU
extern void irq14();     // Primary ATA
extern void irq15();     // Secondary ATA


/*
 * Install IRQ handlers into IDT entries 32-47.
 *
 * IRQ0  -> INT 32
 * IRQ1  -> INT 33
 * ...
 * IRQ15 -> INT 47
 */
void irq_init(){

    idt_set_gate(32, (unsigned int)irq0, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(33, (unsigned int)irq1, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(34, (unsigned int)irq2, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(35, (unsigned int)irq3, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(36, (unsigned int)irq4, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(37, (unsigned int)irq5, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(38, (unsigned int)irq6, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(39, (unsigned int)irq7, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(40, (unsigned int)irq8, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(41, (unsigned int)irq9, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(42, (unsigned int)irq10, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(43, (unsigned int)irq11, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(44, (unsigned int)irq12, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(45, (unsigned int)irq13, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(46, (unsigned int)irq14, KERNEL_CS, IDT_FLAGS);
    idt_set_gate(47, (unsigned int)irq15, KERNEL_CS, IDT_FLAGS);
}


void irq_handler(registers_t* regs){

    /*
     * Send EOI to Slave PIC first.
     *
     * IRQ8-IRQ15 come from the Slave PIC
     * and are mapped to vectors 40-47.
     */
    if(regs->int_no >= 40){
        outb(0xA0, 0x20);
    }

    // Always send EOI to Master PIC.
    outb(0x20, 0x20);

    // Dispatch to registered handler.
    if(interrupt_handlers[regs->int_no]){
        interrupt_handlers[regs->int_no](regs);
    }
}