/*
Description: Catches Interrupt 14 (Page Fault), decodes the hardware error flags, and prints a detailed Kernel Panic screen to help debug Virtual Memory mapping issues.
*/

#include "page_fault.h"
#include "../cpu/interrupt_manager/interrupt_manager.h"
#include "../console/console.h"

// Reads the CPU's Control Register 2 (CR2).
// When a Page Fault occurs, the x86 CPU hardware automatically grabs the exact 32-bit Virtual Address that caused the crash and stuffs it into the CR2 register.
static unsigned int read_cr2(){
    unsigned int value;

    asm volatile(
        "mov %%cr2, %0"
        : "=r"(value)
    );

    return value;
}

static void page_fault_handler(registers_t* regs){
    unsigned int fault_addr = read_cr2();

    printk("\n=== PAGE FAULT ===\n");

    printk("Address : ");
    printk_hex(fault_addr);

    printk("\nError Code : ");
    printk_hex(regs->err_code);

    unsigned int err = regs->err_code;

    printk("\nReason:\n");
    if(err & 1) printk("Protection Violation\n");
    else printk("Page Not Present\n");

    if(err & 2) printk("Write Access\n");
    else printk("Read Access\n");

    if(err & 4) printk("User Mode\n");
    else printk("Kernel Mode\n");

    printk("\n");

    while(1);
}

void page_fault_init(){
    register_interrupt_handler(14, page_fault_handler);
}