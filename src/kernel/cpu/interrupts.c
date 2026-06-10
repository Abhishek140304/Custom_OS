/*
Description: The master dispatcher for CPU exceptions. When an exception occurs, the CPU jumps here with a full snapshot of the registers. We use that data to identify and print the error type.
*/

#include "interrupts.h"
#include "../console/console.h"

const char* exception_messages[] = {
    "Division By Zero",                   // 0
    "Debug",                              // 1
    "Non Maskable Interrupt",             // 2
    "Breakpoint",                         // 3
    "Overflow",                           // 4
    "Bound Range Exceeded",               // 5
    "Invalid Opcode",                     // 6
    "Device Not Available",               // 7
    "Double Fault",                       // 8
    "Coprocessor Segment Overrun",        // 9
    "Invalid TSS",                        // 10
    "Segment Not Present",                // 11
    "Stack Fault",                        // 12
    "General Protection Fault",           // 13
    "Page Fault",                         // 14
    "Reserved",                           // 15
    "Floating Point Error",               // 16
    "Alignment Check",                    // 17
    "Machine Check",                      // 18
    "SIMD Floating Point Exception",      // 19
    "Virtualization Exception",           // 20
    "Control Protection Exception",       // 21
    "Reserved",                           // 22
    "Reserved",                           // 23
    "Reserved",                           // 24
    "Reserved",                           // 25
    "Reserved",                           // 26
    "Reserved",                           // 27
    "Hypervisor Injection Exception",     // 28
    "VMM Communication Exception",        // 29
    "Security Exception",                 // 30
    "Reserved"                            // 31
};

void isr_handler(registers_t* regs){
    printk("CPU Exception: ");

    printk("Interrupt Number: ");
    printk_hex(regs->int_no);
    printk("\n");

    printk("Message: ");
    if(regs->int_no < 32){
        printk(exception_messages[regs->int_no]);
    }
    printk("\n");

    printk("EIP: ");
    printk_hex(regs->eip);
    printk("\n");

    printk("CS: ");
    printk_hex(regs->cs);
    printk("\n");

    printk("EFLAGS: ");
    printk_hex(regs->eflags);
    printk("\n");

    printk("ERROR CODE: ");
    printk_hex(regs->err_code);
    printk("\n");


    while(1);
}