/*
Description: The implementation of our bare-metal VGA text driver.It handles mapping 2D screen coordinates (Row/Col) to the 1D physical video memory array, printing characters, and clearing the screen.
*/

#include "vga.h"

// Keep track of where the next character should be printed
int cursor_row = 0;
int cursor_col = 0;

// Fills the entire 80x25 screen with blank spaces.
void vga_clear_screen(){
    char* memory = (char*) VGA_ADDRESS;

    for(int i=0; i<VGA_COLS*VGA_ROWS; i++){
        memory[i*2] = ' ';
        memory[i*2+1] = WHITE_ON_BLACK;
    }

    cursor_col = 0;
    cursor_row = 0;
}

void vga_print_char(char c, int color){
    char* memory = (char*) VGA_ADDRESS;

    if(c == '\n'){
        cursor_row++;
        cursor_col = 0;
        return;
    }

    int offset = (cursor_row * VGA_COLS + cursor_col) * 2;
    memory[offset] = c;
    memory[offset+1] = color;

    cursor_col++;

    if(cursor_col >= VGA_COLS){
        cursor_row++;
        cursor_col = 0;
    }
}

void vga_print_str(const char* str, int color){
    for(int i=0; str[i]!='\0'; i++){
        vga_print_char(str[i], color);
    }
}

void vga_backspace(){
    if(cursor_col == 0 && cursor_row == 0) return;

    if(cursor_col > 0) cursor_col--;
    else{
        cursor_row--;
        cursor_col = VGA_COLS-1;
    }

    int offset = (cursor_row * VGA_COLS + cursor_col) * 2;

    char* memory = (char*)VGA_ADDRESS;

    memory[offset] = ' ';
    memory[offset+1] = WHITE_ON_BLACK;
}