#ifndef COMMANDS_H
#define COMMANDS_H

// It is a pointer to any C function that returns void and takes a single character pointer (the user's arguments) as a parameter.
typedef void (*command_handler_t)(char* args);

// This struct binds a typed string (like "help") to the specific C function that should execute when that string is typed.
typedef struct{
    const char* name;
    command_handler_t handler;
} command_t;

void help_command(char* args);
void about_command(char* args);
void clear_command(char* args);
void ticks_command(char* args);
void uptime_command(char* args);
void echo_command(char* args);
void reboot_command(char* args);
void wait_command(char* args);

extern command_t commands[];
extern const int command_count;

#endif