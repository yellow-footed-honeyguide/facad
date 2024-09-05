#define _XOPEN_SOURCE 700
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "display_utils.h"
#include "file_entry.h"

#define MAX_COLUMNS 4
#define SPACING 2
#define EMOJI_WIDTH 2
#define EMOJI_NAME_SPACING 1

size_t get_utf8_char_width(const char *str) {
    unsigned char c = (unsigned char)*str;
    if (c < 0x80) return 1;
    if (c < 0xE0) return 2;
    if (c < 0xF0) return 3;
    return 4;
}

size_t get_display_width(const char *str) {
    size_t width = 0;
    while (*str) {
        size_t char_width = get_utf8_char_width(str);
        if (char_width == 4) {
            width += 2;  // Emoji and other wide characters
        } else if (char_width > 1) {
            width += 1;  // Other multi-byte characters
        } else {
            width += 1;  // ASCII characters
        }
        str += char_width;
    }
    return width;
}

void print_padded(const char *str, size_t width) {
    size_t str_width = get_display_width(str);
    printf("%s", str);
    for (size_t i = str_width; i < width; i++) {
        printf(" ");
    }
}

void display_entries(FileEntry *entries, int num_entries, int term_width) {
    setlocale(LC_ALL, "");

    qsort(entries, num_entries, sizeof(FileEntry), compare_file_entries);

    size_t *entry_widths = malloc(num_entries * sizeof(size_t));
    if (!entry_widths) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    size_t max_width = 0;
    for (int i = 0; i < num_entries; i++) {
        entry_widths[i] = EMOJI_WIDTH + EMOJI_NAME_SPACING + get_display_width(entries[i].name);
        if (entry_widths[i] > max_width) {
            max_width = entry_widths[i];
        }
    }

    int num_columns = (term_width + SPACING) / (max_width + SPACING);
    if (num_columns > MAX_COLUMNS) num_columns = MAX_COLUMNS;
    if (num_columns < 1) num_columns = 1;

    size_t *column_widths = calloc(num_columns, sizeof(size_t));
    if (!column_widths) {
        fprintf(stderr, "Memory allocation failed\n");
        free(entry_widths);
        return;
    }

    int rows = (num_entries + num_columns - 1) / num_columns;
    for (int i = 0; i < num_entries; i++) {
        int col = i / rows;
        if (entry_widths[i] > column_widths[col]) {
            column_widths[col] = entry_widths[i];
        }
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < num_columns; col++) {
            int index = col * rows + row;
            if (index < num_entries) {
                printf("%s", entries[index].emoji);
                printf(" ");
                print_padded(entries[index].name, column_widths[col] - EMOJI_WIDTH - EMOJI_NAME_SPACING);
                if (col < num_columns - 1) {
                    printf("%*s", SPACING, "");
                }
            }
        }
        printf("\n");
    }

    free(entry_widths);
    free(column_widths);
}
