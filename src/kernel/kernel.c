# include "vga.h"
#include "console.h"

void main() {
    // clear the screen
    vga_clear_screen();

    printk("Welcome to Custom OS!\n");
    printk("VGA drive initialized successfully.\n");
    printk("Boot sequence complete.\n");
}