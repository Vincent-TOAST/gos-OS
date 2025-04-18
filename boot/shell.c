
#include "shell.h"
#include "timer.h"
#include "keyboard.h"
#include "pci.h"
#include "memory.h"
#include "utils.h"
#include "fs.h"
#include "types.h"

#include <stdint.h>
#include <string.h>

extern char get_char_from_buffer();

char current_path[64] = "/";

char getchar() {
    char c;
    while (!(c = get_char_from_buffer()));
    return c;
}

void read_string(char* buffer, int size) {
    int i = 0;
    char ch;

    while (i < size - 1) {
        ch = getchar();
        if (ch == '\n') {
            print_string("\n");
            break;
        }

        if (ch == '\b') {
            if (i > 0) {
                i--;
                print_string("\b \b");
            }
        } else {
            buffer[i++] = ch;
            char s[2] = {ch, 0};
            print_string(s);
        }
    }
    buffer[i] = '\0';
}

void start_shell(multiboot_info_t* mbi) {
    char input[128];
    char* argv[8];
    int argc;

    while (1) {
        print_colored("gos> ", 0x04);

        read_string(input, sizeof(input));
        argc = split_string(input, argv, 8);
        if (argc == 0) continue;

        print_string(current_path);
        print_string("> ");

        if (strcmp(argv[0], "help") == 0) {
            print_colored("Commands: help, clear, about, pcilist, memmap, uptime, ls, cd, pwd, lick, run\n", 0x04);

        } else if (strcmp(argv[0], "clear") == 0) {
            clear_screen();

        } else if (strcmp(argv[0], "about") == 0) {
            print_colored("gos — a hobby OS project!\n", 0x04);

        } else if (strcmp(argv[0], "pcilist") == 0) {
            scan_pci();

        } else if (strcmp(argv[0], "memmap") == 0) {
            multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mbi->mmap_addr;
            uint32_t mmap_end = mbi->mmap_addr + mbi->mmap_length;

            print_colored("Memory Map:\n", 0x04);
            while ((uint32_t)mmap < mmap_end) {
                print_colored("Base: ", 0x04); print_hex((uint32_t)(mmap->base_addr));
                print_string(" | Length: "); print_hex((uint32_t)(mmap->length));
                print_string(" | Type: "); print_dec(mmap->type); print_string("\n");

                mmap = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
            }

        } else if (strcmp(argv[0], "uptime") == 0) {
            print_colored("Ticks since boot: ", 0x04);
            print_dec(get_ticks());
            print_string("\n");

        } else if (strcmp(argv[0], "ls") == 0) {
            fs_list(current_path);

        } else if (strcmp(argv[0], "cd") == 0 && argc > 1) {
            if (argv[1][0] == '/') {
                strncpy(current_path, argv[1], sizeof(current_path));
            } else {
                strncat(current_path, "/", sizeof(current_path) - strlen(current_path) - 1);
                strncat(current_path, argv[1], sizeof(current_path) - strlen(current_path) - 1);
            }

            print_colored("Changed directory to: ", 0x04);
            print_string(current_path);
            print_string("\n");

        } else if (strcmp(argv[0], "pwd") == 0) {
            print_colored("Current directory: ", 0x04);
            print_string(current_path);
            print_string("\n");

        } else if (strcmp(argv[0], "lick") == 0 && argc > 1) {
            char full_path[64];
            strncpy(full_path, current_path, sizeof(full_path));
            if (current_path[strlen(current_path) - 1] != '/')
                strncat(full_path, "/", sizeof(full_path) - strlen(full_path) - 1);
            strncat(full_path, argv[1], sizeof(full_path) - strlen(full_path) - 1);

            if (fs_create(full_path, 1)) {
                print_colored("Created file: ", 0x04);
                print_string(full_path);
                print_string("\n");
            } else {
                print_colored("Failed to create file.\n", 0x04);
            }

        } else if (strcmp(argv[0], "run") == 0 && argc > 1) {
            void* mod = malloc(4096);
            if (fs_load(argv[1], mod)) {
                print_colored("Loaded module to memory.\n", 0x04);
            } else {
                print_colored("Module not found.\n", 0x04);
            }

        } else {
            print_colored("Unknown command.\n", 0x04);
        }
    }
}
