
#pragma once
#include "types.h"
#ifndef FS_H
#define FS_H

int fs_create(const char* path, uint32_t size);
void fs_list(const char* path);
int fs_load(const char* path, void* buffer);



void fs_init();
void fs_list(const char* path);
int fs_create(const char* full_name, uint32_t size);

#endif