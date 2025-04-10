
#pragma once

#include "types.h"

void start_shell(multiboot_info_t* mbi);

void read_string(char* buffer, int size);

char getchar();
