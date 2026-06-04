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