/*
Description: Implements the high-level console output functions. This acts as a wrapper around the low-level VGA driver, providing a standard, easy-to-use interface for printing text in the kernel.
*/

#include "console.h"
#include "../drivers/vga.h"
#include "../../lib/string.h"

void printk(const char* str){
    vga_print_str(str, WHITE_ON_BLACK);
}

void printk_hex(unsigned int num){
    char buffer[11];
    int_to_hex(num, buffer);
    printk(buffer);
}