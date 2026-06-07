/*
Description: The master dispatcher for CPU exceptions. When an exception occurs, the CPU jumps here with a full snapshot of the registers. We use that data to identify and print the error type.
*/

#include "interrupts.h"
#include "../console/console.h"

const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "Floating Point Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point"
};

void isr_handler(registers_t* regs){
    printk("Exception: ");

    if(regs->int_no < 20){
        printk(exception_messages[regs->int_no]);
    }

    printk("\n");

    while(1);
}