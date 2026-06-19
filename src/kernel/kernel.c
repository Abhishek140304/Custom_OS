/*
Description: The main C entry point for the operating system. It initializes core hardware drivers, sets up the Interrupt Descriptor Table (IDT), and safely halts the CPU into an infinite loop to keep the OS running.
*/


# include "./drivers/vga.h"
#include "./console/console.h"
#include "./cpu/idt/idt.h"
#include "./cpu/irq/pic.h"
#include "./cpu/irq/irq.h"
#include "./drivers/keyboard.h"
#include "./drivers/timer.h"
#include "./shell/shell.h"
#include "./memory/pmm.h"

void kernel_main() {

    vga_clear_screen();

    // printk("Initializing IDT...\n");
    idt_init();

    // printk("Remapping PIC...\n");
    pic_remap();

    // printk("Installing IRQs...\n");
    irq_init();

    // printk("Initializing Timer...\n");
    timer_init();

    // printk("Initializing Keyboard...\n");
    keyboard_init();

    // printk("Initializing Physical Memory Map");
    pmm_init();

    // printk("Enabling Interrupts...\n");
    __asm__ volatile("sti");

    printk("Welcome to MyOs...\n");
    shell_start();
}