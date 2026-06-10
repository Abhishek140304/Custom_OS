#include "string.h"

int strlen(const char* str){
    int len = 0;

    while(str[len] != '\0'){
        len++;
    }

    return len;
}


void int_to_hex(unsigned int num, char* str){
    char* hex = "0123456789ABCDEF";

    str[0] = '0';
    str[1] = 'x';

    for(int i=0; i<8; i++){
        str[9-i] = hex[num & 0xF];
        num >>= 4;
    }

    str[10] = '\0';
}