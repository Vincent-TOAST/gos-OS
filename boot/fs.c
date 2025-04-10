
#include "fs.h"
#include "utils.h"
#include "types.h"
#include <string.h>

fs_entry_t file_table[MAX_FILES];

void fs_list(const char* path) {
    print_colored("Directory: ", 0x04);
    print_string(path);
    print_string("\n");

    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].name[0] != 0) {
            print_string(" - ");
            print_string(file_table[i].name);
            print_string("\n");
        }
    }
}

int fs_create(const char* path, uint32_t size) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].name[0] == 0) {
            strncpy(file_table[i].name, path, sizeof(file_table[i].name));
            file_table[i].start_sector = 1 + i;
            file_table[i].size = size;
            return 1;
        }
    }
    return 0;
}

void fs_init() {
}
