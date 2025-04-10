
#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define HEAP_START 0x100000  // 1MB
#define HEAP_SIZE  0x10000   // 64KB

typedef struct block_header {
    uint32_t size;
    int free;
    struct block_header* next;
} block_header_t;

extern block_header_t* heap_base;
extern int heap_initialized;

void* malloc(uint32_t size);
void free(void* ptr);
void init_heap();

#endif
