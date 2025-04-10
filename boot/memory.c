
#include "memory.h"


block_header_t* heap_base = (block_header_t*)HEAP_START;
int heap_initialized = 0;

void init_heap() {
    heap_base->size = HEAP_SIZE - sizeof(block_header_t);
    heap_base->free = 1;
    heap_base->next = 0;
    heap_initialized = 1;
}

void* malloc(uint32_t size) {
    if (!heap_initialized)
        init_heap();

    block_header_t* curr = heap_base;

    while (curr) {
        if (curr->free && curr->size >= size) {
            if (curr->size > size + sizeof(block_header_t)) {
                block_header_t* new_block = (void*)((char*)curr + sizeof(block_header_t) + size);
                new_block->size = curr->size - size - sizeof(block_header_t);
                new_block->free = 1;
                new_block->next = curr->next;
                curr->next = new_block;
                curr->size = size;
            }
            curr->free = 0;
            return (void*)(curr + 1);
        }
        curr = curr->next;
    }

    return 0; 
}

void free(void* ptr) {
    if (!ptr) return;

    block_header_t* block = (block_header_t*)ptr - 1;
    block->free = 1;

    if (block->next && block->next->free) {
        block->size += sizeof(block_header_t) + block->next->size;
        block->next = block->next->next;
    }
}
