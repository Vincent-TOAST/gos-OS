
#pragma once
#include <stdint.h>

void print_hex(uint32_t value);

int split_string(char* input, char* argv[], int max_args);

void print_string(const char* str);
void print_colored(const char* str, uint8_t color);
void print_dec(uint32_t value);
void print_string(const char* str);
void read_string(char* buffer, int size);

void clear_screen(void);