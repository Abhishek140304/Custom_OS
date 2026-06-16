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

void printk_char(char c){
    vga_print_char(c, WHITE_ON_BLACK);
}

void printk_dec(unsigned int n){
    if(n == 0){
        printk_char('0');
        return;
    }

    char digits[16];
    int i = 0;
    while(n > 0){
        digits[i++] = (n % 10) + '0';
        n /= 10;
    }

    while(i > 0){
        printk_char(digits[--i]);
    }
}