/*
Description: Initializes hardware interrupts by populating the IDT with assembly wrappers. It also provides the master C handler to route hardware signals and send End-Of-Interrupt (EOI) commands.
*/

#include "irq.h"
#include "../idt.h"
#include "ports.h"
#include "../../drivers/timer.h"

extern void irq0();     // System Timer
extern void irq1();     // Keyboard
extern void irq2();     // Cascade (Internal PIC communication)
extern void irq3();     // COM2 (Serial Port)
extern void irq4();     // COM1 (Serial Port)
extern void irq5();     // LPT2 (Parallel Port)
extern void irq6();     // Floppy Disk
extern void irq7();     // LPT1 (Parallel Port)
extern void irq8();     // Real-Time Clock
extern void irq9();     // General Peripherals / Legacy SCSI
extern void irq10();    // General Peripherals / Legacy SCSI
extern void irq11();    // General Peripherals / Legacy SCSI
extern void irq12();    // PS/2 Mouse
extern void irq13();    // FPU (Math Coprocessor)
extern void irq14();    // Primary ATA Hard Disk
extern void irq15();    // Secondary ATA Hard Disk


void irq_init(){

    idt_set_gate(32, (unsigned int)irq0,  0x08, 0x8E);
    idt_set_gate(33, (unsigned int)irq1,  0x08, 0x8E);
    idt_set_gate(34, (unsigned int)irq2,  0x08, 0x8E);
    idt_set_gate(35, (unsigned int)irq3,  0x08, 0x8E);
    idt_set_gate(36, (unsigned int)irq4,  0x08, 0x8E);
    idt_set_gate(37, (unsigned int)irq5,  0x08, 0x8E);
    idt_set_gate(38, (unsigned int)irq6,  0x08, 0x8E);
    idt_set_gate(39, (unsigned int)irq7,  0x08, 0x8E);
    idt_set_gate(40, (unsigned int)irq8,  0x08, 0x8E);
    idt_set_gate(41, (unsigned int)irq9,  0x08, 0x8E);
    idt_set_gate(42, (unsigned int)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned int)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned int)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned int)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned int)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned int)irq15, 0x08, 0x8E);
}


void irq_handler(registers_t* regs){

    if(regs->int_no == 32){
        timer_callback();
    }

    // Send the End-Of-Interrupt (EOI) Signal. We must tell the PIC (Programmable Interrupt Controller) that we have successfully handled the interrupt. If we don't, the PIC will freeze and stop sending us ANY future hardware signals.


    /* Slave PIC */
    if(regs->int_no >= 40){
        outb(0xA0, 0x20);
    }

    /* Master PIC */
    outb(0x20, 0x20);
}