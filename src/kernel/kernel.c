/*
Description: The main C entry point for the operating system. It initializes core hardware drivers, sets up the Interrupt Descriptor Table (IDT), and safely halts the CPU into an infinite loop to keep the OS running.
*/


# include "./drivers/vga.h"
#include "./console/console.h"
#include "./cpu/idt.h"

void kernel_main() {
    // clear the screen
    vga_clear_screen();

    printk("Initialising IDT...\n");
    
    idt_init();

    printk("IDT loaded\n");

    __asm__ volatile("int $19");

    while(1);
}