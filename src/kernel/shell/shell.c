/*
Description: The primary interactive REPL (Read-Eval-Print Loop) for the operating system. It fetches keystrokes, parses strings in-place, and dispatches execution to registered command handlers.
*/

#include "shell.h"
#include "../console/console.h"
#include "../drivers/keyboard.h"
#include "../../lib/string.h"
#include "commands.h"

// Takes a raw input string (e.g., "echo hello") and cleanly splits it into a command ("echo") and arguments ("hello")
void parse_command(char* input, char** command, char** args){
    *command = input;
    *args = 0;

    while(*input){
        if(*input == ' '){
            *input = '\0';
            *args = input + 1;

            return;
        }

        input++;
    }
}

// Looks up the parsed command in our global registry and triggers the corresponding function.
static void execute_command(char* cmd, char* args){
    for(int i=0; i<command_count; i++){
        if(strcmp(cmd, commands[i].name) == 0){
            commands[i].handler(args);
            return;
        }
    }
    printk("\nUnknown command: ");
    printk(cmd);
    printk("\n");
}

void shell_start(){
    char input[128];

    while(1){
        printk("\n> ");
    
        keyboard_readline(input, 128);
    
        char* command;
        char* args;
        parse_command(input, &command, &args);

        execute_command(command, args);
    }

}