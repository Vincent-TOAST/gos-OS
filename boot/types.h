
#pragma once
#include <stdint.h>  

#define MAX_FILENAME 32
#define MAX_FILES 128

typedef struct {
    char name[MAX_FILENAME];
    uint32_t start_sector;
    uint32_t size;
    int is_dir;
} fs_entry_t;

typedef struct {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved;
} multiboot_module_t;

typedef struct {
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed)) multiboot_memory_map_t;

typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t mmap_length;
    uint32_t mmap_addr;
} multiboot_info_t;
