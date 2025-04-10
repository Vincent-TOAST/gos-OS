
#include "utils.h" 
#include <stddef.h>

size_t strlen(const char* str) {
    size_t len = 0;
    while (*str++) len++;
    return len;
}

char* strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
    return dest;
}

char* strncat(char* dest, const char* src, size_t n) {
    size_t dlen = strlen(dest);
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[dlen + i] = src[i];
    dest[dlen + i] = '\0';
    return dest;
}


void print_hex(uint32_t value) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[11]; 
    buffer[0] = '0';
    buffer[1] = 'x';
    buffer[10] = '\0';

    for (int i = 0; i < 8; i++) {
        uint8_t nibble = (value >> (28 - i * 4)) & 0xF;
        buffer[2 + i] = hex_chars[nibble];
    }

    print_colored(buffer, 0x0B);
}

int split_string(char* input, char* argv[], int max_args) {
    int argc = 0;

    while (*input && argc < max_args) {
     
        while (*input == ' ') input++;

        if (*input == '\0') break;

        argv[argc++] = input;

       
        while (*input && *input != ' ') input++;
        if (*input) {
            *input = '\0';
            input++;
        }
    }

    return argc;
}

void print_dec(uint32_t value) {
    char buffer[12]; 
    int i = 0;

    if (value == 0) {
        print_string("0");
        return;
    }

    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    buffer[i] = '\0';

  
    for (int j = 0; j < i / 2; j++) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - 1 - j];
        buffer[i - 1 - j] = tmp;
    }

    print_string(buffer);
}

void print_string(const char* str) {
    print_colored(str, 0x04);  
}


