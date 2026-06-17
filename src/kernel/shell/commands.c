/*
Description: The application layer of the OS. Contains the array of registered shell commands and the C functions that execute them.
*/

#include "commands.h"
#include "../console/console.h"
#include "../drivers/vga.h"
#include "../drivers/timer.h"
#include "../cpu/io/ports.h"
#include "../../lib/string.h"
#include "../memory/memory_info.h"
#include "../memory/e280.h"

command_t commands[] = {
    {"help", help_command},
    {"about", about_command},
    {"clear", clear_command},
    {"cls", clear_command},
    {"ticks", ticks_command},
    {"uptime", uptime_command},
    {"echo", echo_command},
    {"reboot", reboot_command},
    {"wait", wait_command},
    {"meminfo", meminfo_command},
    {"memmap", memmap_command},
    {"raminfo", raminfo_command}
};

const int command_count = sizeof(commands) / sizeof(command_t);

void help_command(char* args){
    printk("\nAvailable Commands:\n");
    
    for(int i=0; i<command_count; i++){
        printk(commands[i].name);
        printk("\n");
    }

    return;
}

void about_command(char* args){
    printk("\nMyOS\n");     
    printk("Built from scratch in C and Assembly\n");

    return;
}

void clear_command(char* args){
    vga_clear_screen();

    return;
}

void ticks_command(char* args){
    printk("\nTicks: \n");
    printk_dec(timer_get_ticks());
    printk("\n");
}

void uptime_command(char* args){
    unsigned int seconds = timer_get_ticks() / 100;

    printk("\nUptime: ");
    printk_dec(seconds);
    printk(" seconds\n");
}

void echo_command(char* args){
    printk("\n");

    if(args) printk(args);

    printk("\n");
}

void reboot_command(char* args){
    printk("\nRebooting...\n");

    // Wait for the keyboard controller to finish any current tasks. Reading port 0x64 gets the status byte. Bit 1 (0x02) is the "Input Buffer Full" flag.
    while((inb(0x64) & 0x02) != 0);

    // Send 0xFE (reboot command) to the keyboard command port (0x64).
    outb(0x64, 0xFE);

    while(1);
}

void wait_command(char* args){
    printk("\nWaiting 3 seconds...\n");

    sleep(3000);

    printk("Done!\n");
}

void meminfo_command(char* args){
    memory_info_print();
}

void memmap_command(char* args){
    e820_print_map();
}

void raminfo_command(char* args){
    printk("\nUsable RAM: ");
    printk_hex(e820_total_usable_ram());

    printk("\nReserved RAM: ");
    printk_hex(e820_total_reserved_ram());

    printk("\nLargest RAM: ");
    printk_hex(e820_largest_usable_region());

    printk("\n");
}