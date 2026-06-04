#include "console.h"
#include "../drivers/vga.h"

void printk(const char* str){
    vga_print_str(str, WHITE_ON_BLACK);
}