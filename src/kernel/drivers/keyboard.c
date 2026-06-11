/*
Description: The PS/2 Keyboard Driver. It reads raw scancodes from hardware port 0x60, translates them to ASCII, and stores them in a circular buffer for the OS to consume
*/


#include "keyboard.h"
#include "../cpu/io/ports.h"
#include "../console/console.h"
#include "vga.h"
#include "../cpu/interrupt_manager/interrupt_manager.h"

#define BUFFER_SIZE 256

static char keyboard_buffer[BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;

static int shift_pressed = 0;


// Keyboards don't send letters; they send physical key coordinates (Scancodes). For example, pressing the 'Q' key sends the number 16. We use these arrays to map the raw number 16 to the ASCII char 'q'.
static char normal_map[128] = {
    0,
    27,
    '1','2','3','4','5','6','7','8','9','0',
    '-','=',
    '\b',
    '\t',
    'q','w','e','r','t','y','u','i','o','p',
    '[',']',
    '\n',
    0,
    'a','s','d','f','g','h','j','k','l',
    ';','\'','`',
    0,
    '\\',
    'z','x','c','v','b','n','m',
    ',', '.', '/',
    0,
    '*',
    0,
    ' '
};

static char shift_map[128] = {
    0,
    27,
    '!','@','#','$','%','^','&','*','(',')',
    '_','+',
    '\b',
    '\t',
    'Q','W','E','R','T','Y','U','I','O','P',
    '{','}',
    '\n',
    0,
    'A','S','D','F','G','H','J','K','L',
    ':','"','~',
    0,
    '|',
    'Z','X','C','V','B','N','M',
    '<','>','?',
    0,
    '*',
    0,
    ' '
};

static void buffer_put(char c){
    keyboard_buffer[buffer_head] = c;
    buffer_head = (buffer_head + 1) % BUFFER_SIZE;
}

static void buffer_pop(){
    if(buffer_head == buffer_tail) return;

    buffer_head = (buffer_head - 1 + BUFFER_SIZE) % BUFFER_SIZE;
}

char keyboard_get_char(){
    if(buffer_head == buffer_tail) return 0;

    char c = keyboard_buffer[buffer_tail];

    buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;

    return c;
}

// This function is triggered by the PIC every time a key is pressed OR released.
static void keyboard_callback(registers_t* regs){
    // Port 0x60 is the PS/2 Keyboard Controller's data port. Reading from it pulls the raw scancode from the hardware.
    unsigned char scancode = inb(0x60);

    /* Key release */
    // In PS/2 protocol, releasing a key sends the same scancode + 0x80 (128). The bitwise AND (& 0x80) checks if the highest bit is set to 1.
    if(scancode & 0x80){
        if(scancode == 170 || scancode == 182) shift_pressed = 0;
        return;
    }

    /* Shift press */
    if(scancode == 42 || scancode == 54){
        shift_pressed = 1;
        return;
    }

    char c;

    if(shift_pressed) c = shift_map[scancode];
    else c = normal_map[scancode];

    if(!c) return;

    if(c == '\n'){
        printk_char('\n');
        buffer_put('\n');
        return;
    }

    if(c == '\b'){
        vga_backspace();
        buffer_pop();
        return;
    }

    printk_char(c);
    buffer_put(c);
}


void keyboard_init(){
    register_interrupt_handler(33, keyboard_callback);
}