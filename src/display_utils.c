/**
 * @file display_utils.c
 * @brief Implementation of display utilities for file entries.
 *
 * This file contains functions for formatting and displaying file entries
 * in a grid layout, taking into account terminal width and Unicode characters.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

// Enable POSIX.1-2008 + XSI (X/Open System Interfaces Extension) features
#define _XOPEN_SOURCE 700

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <glob.h>

#include "display_utils.h"
#include "file_card.h"
#include "dir_config.h"
#include "git_utils.h"

// Constants for display formatting
#define MAX_COLUMNS 4
#define SPACING 2
#define EMOJI_WIDTH 2
#define EMOJI_NAME_SPACING 1

/**
 * @brief Determines the byte width of a UTF-8 character.
 *
 * @param str Pointer to the start of a UTF-8 character.
 * @return The number of bytes used by the character (1-4).
 */
size_t get_utf8_char_width(const char *str) {
    unsigned char c = (unsigned char)*str;
    if (c < 0x80) return 1;  // ASCII character
    if (c < 0xE0) return 2;  // 2-byte UTF-8 character
    if (c < 0xF0) return 3;  // 3-byte UTF-8 character
    return 4;                // 4-byte UTF-8 character
}

/**
 * @brief Calculates the display width of a string, accounting for multi-byte characters.
 *
 * @param str The string to measure.
 * @return The display width of the string.
 */
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

/**
 * @brief Prints a string and pads it with spaces to reach the specified width.
 *
 * @param str The string to print.
 * @param width The total width to fill, including the string and padding.
 */
void print_padded(const char *str, size_t width) {
    size_t str_width = get_display_width(str);
    printf("%s", str);
    // Add padding spaces
    for (size_t i = str_width; i < width; i++) {
        printf(" ");
    }
}

/**
 * @brief Displays file entries in a formatted, multi-column layout.
 *
 * This function sorts the entries, calculates the appropriate layout based on
 * terminal width, and prints the entries in a grid format.
 *
 * @param entries Array of FileCardInfo structures to display.
 * @param num_entries Number of entries in the array.
 * @param term_width Width of the terminal in characters.
 * @param current_dir Current directory path.
 * @param show_path Flag to indicate whether to show the current path.
 */
void display_entries(FileCardInfo *entries, int num_entries, int term_width,
                     const char *current_dir, int show_path) {
    setlocale(LC_ALL, "");

    if (show_path) {
        char *branch = get_current_branch();
        if (branch) {
            printf("\033[1m%s\033[0m (\033[32m%s\033[0m)\n", current_dir, branch);
            free(branch);
        } else {
            printf("\033[1m%s\033[0m\n", current_dir);
        }
    }

    size_t *entry_widths = malloc(num_entries * sizeof(size_t));
    if (!entry_widths) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    size_t max_width = 0;
    for (int i = 0; i < num_entries; i++) {
        entry_widths[i] = EMOJI_WIDTH + EMOJI_NAME_SPACING + get_display_width(entries[i].name);
        if (entries[i].git_status[0] != '\0') {
            entry_widths[i] += 3; // Add 3 for "(X)"
        }
        if (entry_widths[i] > max_width) {
            max_width = entry_widths[i];
        }
    }

    int num_columns = (term_width + SPACING) / (max_width + SPACING);
    int max_columns = get_max_columns(current_dir);
    if (num_columns > max_columns) num_columns = max_columns;
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
                printf("%s ", entries[index].emoji);
                printf("%s", entries[index].name);

                if (entries[index].git_status[0] != '\0') {
                    printf("(\033[32m%c\033[0m)", entries[index].git_status[0]);
                }

                size_t printed_width = entry_widths[index];
                size_t padding = (col < num_columns - 1) ? column_widths[col] - printed_width + SPACING : 0;
                for (size_t i = 0; i < padding; i++) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    free(entry_widths);
    free(column_widths);
}
