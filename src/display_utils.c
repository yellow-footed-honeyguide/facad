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

#include "display_utils.h"
#include "file_card.h"
#include "dir_config.h"

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
 */
void display_entries(FileCardInfo *entries, int num_entries, int term_width,
                     const char *current_dir) {
    // Set the locale to the user's default for proper wide character handling
    setlocale(LC_ALL, "");

    // Sort the entries
    qsort(entries, num_entries, sizeof(FileCardInfo), compare_file_entries);

    // Allocate memory for storing entry widths
    size_t *entry_widths = malloc(num_entries * sizeof(size_t));
    if (!entry_widths) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Calculate the width of each entry and find the maximum width
    size_t max_width = 0;
    for (int i = 0; i < num_entries; i++) {
        entry_widths[i] = EMOJI_WIDTH + EMOJI_NAME_SPACING + get_display_width(entries[i].name);
        if (entry_widths[i] > max_width) {
            max_width = entry_widths[i];
        }
    }

    // Calculate the number of columns based on terminal width
     int num_columns = (term_width + SPACING) / (max_width + SPACING);
    int max_columns = get_max_columns(current_dir);
    if (num_columns > max_columns) num_columns = max_columns;
    if (num_columns < 1) num_columns = 1;

    // Allocate memory for storing column widths
    size_t *column_widths = calloc(num_columns, sizeof(size_t));
    if (!column_widths) {
        fprintf(stderr, "Memory allocation failed\n");
        free(entry_widths);
        return;
    }

    // Calculate the number of rows needed
    int rows = (num_entries + num_columns - 1) / num_columns;

    // Determine the width of each column
    for (int i = 0; i < num_entries; i++) {
        int col = i / rows;
        if (entry_widths[i] > column_widths[col]) {
            column_widths[col] = entry_widths[i];
        }
    }

    // Display the entries in a grid format
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < num_columns; col++) {
            int index = col * rows + row;
            if (index < num_entries) {
                // Print emoji
                printf("%s", entries[index].emoji);
                printf(" ");
                // Print filename with padding
                print_padded(entries[index].name,
                             column_widths[col] - EMOJI_WIDTH - EMOJI_NAME_SPACING);
                // Add spacing between columns
                if (col < num_columns - 1) {
                    printf("%*s", SPACING, "");
                }
            }
        }
        printf("\n");
    }

    // Free allocated memory
    free(entry_widths);
    free(column_widths);
}
