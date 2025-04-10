
#pragma once
#include "types.h"

void fs_init();
void fs_list(const char* path);
int fs_create(const char* full_name, uint32_t size);
