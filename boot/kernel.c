
#include "types.h"
#include "fs.h"
#include "shell.h"
#include "memory.h"
#include "utils.h"
#include "pci.h"
#include "idt.h"
#include "timer.h"
#include "keyboard.h"

#include <stdint.h>
#include <string.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VIDEO_MEMORY (char*)0xB8000

static int row = 0, col = 0;

void clear_screen() {
    volatile char* video = VIDEO_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = 0x07;
    }
    row = 0;
    col = 0;
}

void scroll_screen() {
    char* video = VIDEO_MEMORY;
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            int from = (y * VGA_WIDTH + x) * 2;
            int to = ((y - 1) * VGA_WIDTH + x) * 2;
            video[to] = video[from];
            video[to + 1] = video[from + 1];
        }
    }

    for (int x = 0; x < VGA_WIDTH; x++) {
        int offset = ((VGA_HEIGHT - 1) * VGA_WIDTH + x) * 2;
        video[offset] = ' ';
        video[offset + 1] = 0x07;
    }

    row = VGA_HEIGHT - 1;
    col = 0;
}

void print_colored(const char* str, uint8_t color) {
    volatile char* video = VIDEO_MEMORY;
    while (*str) {
        char c = *str++;
        if (c == '\n') {
            row++;
            col = 0;
        } else {
            int offset = 2 * (row * VGA_WIDTH + col);
            video[offset] = c;
            video[offset + 1] = color;
            col++;
            if (col >= VGA_WIDTH) {
                col = 0;
                row++;
            }
        }
        if (row >= VGA_HEIGHT) {
            scroll_screen();
        }
    }
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void kernel_main(multiboot_info_t* mbi) {
    idt_init();
    fs_init();
    timer_init();
    keyboard_init();

    clear_screen();
    print_colored("gos OS Booting...\n", 0x04);
    print_colored("Enter Passkey: ", 0x04);

    char passkey[64];
    read_string(passkey, sizeof(passkey));

    if (strcmp(passkey, "letmein") == 0) {
        print_colored("\nAccess granted.\nLaunching shell...\n", 0x04);

        if (mbi->flags & (1 << 3)) {
            multiboot_module_t* mods = (multiboot_module_t*)mbi->mods_addr;
            print_colored("Modules detected:\n", 0x04);
            for (uint32_t i = 0; i < mbi->mods_count; i++) {
                print_colored(" - Module at ", 0x04);
                print_hex(mods[i].mod_start);
                print_colored("\n", 0x04);
            }
        }

        start_shell(mbi);
    } else {
        print_colored("\nAccess Denied.\n", 0x04);
        while (1);
    }
}

__attribute__((section(".multiboot")))
const unsigned int multiboot_header[] = {
    0x1BADB002,
    0x00,
    -(0x1BADB002 + 0x00)
};

