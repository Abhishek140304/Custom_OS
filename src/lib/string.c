/*
Description: Custom kernel string library.
*/

#include "string.h"

// Calculates the length of a string.
int strlen(const char* str){
    int len = 0;

    while(str[len] != '\0'){
        len++;
    }

    return len;
}


// Converts a 32-bit integer into a readable Hexadecimal string
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

// Compares two strings to see if they are identical. Returns 0 if they match completely.
int strcmp(const char* a, const char* b){
    while(*a && *b){
        if(*a != *b) return *a - *b;

        a++;
        b++;
    }

    return *a - *b;
}

// Copies the characters from a source string into a destination buffer.
char* strcpy(char* dest, const char* src){
    char* start = dest;

    while(*src){
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';
    return start;
}