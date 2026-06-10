/*
Description: The main C entry point for the operating system. It initializes core hardware drivers, sets up the Interrupt Descriptor Table (IDT), and safely halts the CPU into an infinite loop to keep the OS running.
*/


# include "./drivers/vga.h"
#include "./console/console.h"
#include "./cpu/idt.h"
#include "./cpu/hardware_exceptions/pic.h"
#include "./cpu/hardware_exceptions/irq.h"

void kernel_main() {
    // clear the screen
    vga_clear_screen();

    printk("Initialising IDT...\n");
    idt_init();

    printk("Remappint PIC...\n");
    pic_remap();
    
    printk("Installing IRQ...\n");
    irq_init();

    // volatile int x = 0;
    // volatile int y = 10 / x;
    // __asm__ volatile("int $8");

    printk("Enabling Interrupts...\n");
    __asm__ volatile("sti");

    printk("Interrupt Enabled!\n");

    while(1);
}