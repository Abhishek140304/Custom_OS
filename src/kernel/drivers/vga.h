/*
Description: Defines the constants, screen dimensions, color codes, and function prototypes for the bare-metal VGA text mode driver.
*/

#ifndef VGA_H
#define VGA_H

// VGA text buffer memory address
#define VGA_ADDRESS  0xb8000

// Standard VGA screen dimensions
#define VGA_COLS 80
#define VGA_ROWS 25

// few color codes
#define WHITE_ON_BLACK 0x0f
#define RED_ON_BLACK 0x04
#define GREEN_ON_BLACK 0x02

// functions
void vga_clear_screen();
void vga_print_str(const char* str, int color);
void vga_print_char(char c, int color);
void vga_backspace();

#endif