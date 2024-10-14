/**
 * @file display_utils.c
 * @brief Implementation of display utilities for file entries.
 *
 * This file contains functions for formatting and displaying file entries
 * in a grid layout, taking into account terminal width and Unicode characters.
 * It handles the visual representation of directory contents, including
 * emojis, Git status, and proper spacing.
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
 * This function analyzes the first byte of a UTF-8 character to determine
 * its total byte width.
 *
 * @param str Pointer to the start of a UTF-8 character.
 * @return The number of bytes used by the character (1-4).
 */
size_t get_utf8_char_width(const char *str) {
    unsigned char c = (unsigned char)*str;  // Get the first byte of the character
    if (c < 0x80) return 1;      // ASCII character (0xxxxxxx)
    if (c < 0xE0) return 2;      // 2-byte UTF-8 character (110xxxxx)
    if (c < 0xF0) return 3;      // 3-byte UTF-8 character (1110xxxx)
    return 4;                    // 4-byte UTF-8 character (11110xxx)
}

/**
 * @brief Calculates the display width of a string, accounting for multi-byte characters.
 *
 * This function iterates through the string, determining the width of each character
 * and summing them to get the total display width.
 *
 * @param str The string to measure.
 * @return The display width of the string.
 */
size_t get_display_width(const char *str) {
    size_t width = 0;
    while (*str) {  // Loop through each character in the string
        size_t char_width = get_utf8_char_width(str);  // Get the byte width of the current character
        if (char_width == 4) {
            width += 2;  // Emoji and other wide characters count as 2 display units
        } else if (char_width > 1) {
            width += 1;  // Other multi-byte characters count as 1 display unit
        } else {
            width += 1;  // ASCII characters count as 1 display unit
        }
        str += char_width;  // Move to the next character
    }
    return width;
}

/**
 * @brief Prints a string and pads it with spaces to reach the specified width.
 *
 * This function prints the given string and adds space padding to ensure
 * consistent column widths in the display.
 *
 * @param str The string to print.
 * @param width The total width to fill, including the string and padding.
 */
void print_padded(const char *str, size_t width) {
    size_t str_width = get_display_width(str);  // Get the display width of the string
    printf("%s", str);  // Print the string
    // Add padding spaces
    for (size_t i = str_width; i < width; i++) {
        printf(" ");
    }
}

/**
 * @brief Displays file entries in a formatted, multi-column layout.
 *
 * This function sorts the entries, calculates the appropriate layout based on
 * terminal width, and prints the entries in a grid format. It handles emojis,
 * Git status, and ensures proper alignment.
 *
 * @param entries Array of FileCardInfo structures to display.
 * @param num_entries Number of entries in the array.
 * @param term_width Width of the terminal in characters.
 * @param current_dir Current directory path.
 * @param show_path Flag to indicate whether to show the current path.
 */
void display_entries(FileCardInfo *entries, int num_entries, int term_width,
                     const char *current_dir, int show_path) {
    setlocale(LC_ALL, "");  // Set the locale for proper Unicode handling

    // Display the current directory and Git branch if requested
    if (show_path) {
        char *branch = get_current_branch();
        if (branch) {
            printf("\033[1m%s\033[0m (\033[32m%s\033[0m)\n", current_dir, branch);
            free(branch);
        } else {
            printf("\033[1m%s\033[0m\n", current_dir);
        }
    }

    // Allocate memory for storing entry widths
    size_t *entry_widths = malloc(num_entries * sizeof(size_t));
    if (!entry_widths) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Calculate the width of each entry and find the maximum width
    size_t max_width = 0;
    for (int i = 0; i < num_entries; i++) {
        // Calculate width: emoji + spacing + name + potential Git status
        entry_widths[i] = EMOJI_WIDTH + EMOJI_NAME_SPACING + get_display_width(entries[i].name);
        if (entries[i].git_status[0] != '\0') {
            entry_widths[i] += 3; // Add 3 for "(X)" Git status
        }
        if (entry_widths[i] > max_width) {
            max_width = entry_widths[i];
        }
    }

    // Calculate the number of columns based on terminal width
    int num_columns = (term_width + SPACING) / (max_width + SPACING);
    int max_columns = get_max_columns(current_dir);
    if (num_columns > max_columns) num_columns = max_columns;
    if (num_columns < 1) num_columns = 1;

    // Allocate memory for column widths
    size_t *column_widths = calloc(num_columns, sizeof(size_t));
    if (!column_widths) {
        fprintf(stderr, "Memory allocation failed\n");
        free(entry_widths);
        return;
    }

    // Calculate the number of rows
    int rows = (num_entries + num_columns - 1) / num_columns;

    // Calculate the width of each column
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
                printf("%s ", entries[index].emoji);
                // Print file name
                printf("%s", entries[index].name);

                // Print Git status if available
                if (entries[index].git_status[0] != '\0') {
                    printf("(\033[32m%c\033[0m)", entries[index].git_status[0]);
                }

                // Calculate and print padding
                size_t printed_width = entry_widths[index];
                size_t padding = (col < num_columns - 1) ? column_widths[col] - printed_width + SPACING : 0;
                for (size_t i = 0; i < padding; i++) {
                    printf(" ");
                }
            }
        }
        printf("\n");  // Move to the next row
    }

    // Free allocated memory
    free(entry_widths);
    free(column_widths);
}
