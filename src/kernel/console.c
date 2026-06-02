#include "console.h"
#include "vga.h"

void printk(char* str){
    vga_print_str(str, WHITE_ON_BLACK);
}