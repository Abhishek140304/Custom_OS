#ifndef KEYBOARD_H 
#define KEYBOARD_H

#include "../cpu/exceptions/interrupts.h"

void keyboard_init();

static void keyboard_callback(registers_t* regs);

char keyboard_get_char();

void keyboard_readline(char* buffer, int max_len);

#endif