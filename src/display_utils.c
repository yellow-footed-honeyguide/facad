/**
 * @file display_utils.c
 * @brief Utility functions for displaying directory entries
 *
 * This file contains functions for formatting and displaying
 * directory entries in a grid layout. It handles UTF-8 character
 * width calculations and Git status integration.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <glob.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#include "file_card.h"
#include "git_utils.h"
#include "dir_config.h"
#include "display_utils.h"


#define MAX_COLUMNS 4
#define SPACING 2
#define EMOJI_WIDTH 2
#define EMOJI_NAME_SPACING 1

/**
 * @brief Calculate the width of a UTF-8 character
 *
 * @param str Pointer to the start of a UTF-8 character
 * @return Size of the UTF-8 character in bytes
 */
size_t get_utf8_char_width(const char *str) {
    unsigned char c = (unsigned char)*str;  // Get the first byte of the character
    if (c < 0x80) return 1;  // ASCII character (1 byte)
    if (c < 0xE0) return 2;  // 2-byte UTF-8 character
    if (c < 0xF0) return 3;  // 3-byte UTF-8 character
    return 4;  // 4-byte UTF-8 character
}

/**
 * @brief Calculate the display width of a UTF-8 string
 *
 * @param str The UTF-8 string to measure
 * @return Display width of the string
 */
size_t get_display_width(const char *str) {
    size_t width = 0;  // Initialize the display width
    while (*str) {  // Loop through each character in the string
        size_t char_width = get_utf8_char_width(str);  // Get the byte width of the current character
        if (char_width == 4) {
            width += 2;  // 4-byte UTF-8 characters are typically displayed as width 2
        } else if (char_width > 1) {
            width += 1;  // 2 and 3-byte UTF-8 characters are typically displayed as width 1
        } else {
            width += 1;  // ASCII characters have a width of 1
        }
        str += char_width;  // Move to the next character
    }
    return width;  // Return the total display width
}

/**
 * @brief Print a string with padding to a specified width
 *
 * @param str The string to print
 * @param width The desired total width
 */
void print_padded(const char *str, size_t width) {
    size_t str_width = get_display_width(str);  // Get the display width of the string
    printf("%s", str);  // Print the string
    for (size_t i = str_width; i < width; i++) {
        printf(" ");  // Add padding spaces
    }
}

/**
 * @brief Display directory entries in a grid layout
 *
 * @param entries Array of FileCardInfo structures
 * @param num_entries Number of entries in the array
 * @param term_width Width of the terminal
 * @param current_dir Current directory path
 * @param show_path Flag to determine if the path should be displayed
 */
void display_entries(FileCardInfo *entries, int num_entries, int term_width,
                     const char *current_dir, int show_path) {
    setlocale(LC_ALL, "");

    if (show_path) {
        char *branch = get_current_branch(current_dir);
        if (branch) {
            printf("\033[1m%s\033[0m  \033[38;5;202m[%s]\033[0m\n", current_dir, branch);  // Print directory and branch
            free(branch);  // Free the allocated branch name
        } else {
            printf("\033[1m%s\033[0m\n", current_dir);  // Print directory without branch
        }
    }

    // Allocate memory for entry widths
    size_t *entry_widths = malloc(num_entries * sizeof(size_t));
    if (!entry_widths) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    size_t max_width = 0;  // Track the maximum entry width
    for (int i = 0; i < num_entries; i++) {
        // Calculate the width of each entry
        entry_widths[i] = EMOJI_WIDTH + EMOJI_NAME_SPACING + get_display_width(entries[i].name);
        if (entries[i].git_status[0] != '\0' || (entries[i].is_directory && entries[i].contains_modified)) {
            entry_widths[i] += 3;  // Add space for Git status
        }
        if (entry_widths[i] > max_width) {
            max_width = entry_widths[i];  // Update maximum width
        }
    }

    // Calculate the number of columns based on terminal width
    int num_columns = (term_width + SPACING) / (max_width + SPACING);
    int max_columns = get_max_columns(current_dir);  // Get max columns for the current directory
    if (num_columns > max_columns) num_columns = max_columns;
    if (num_columns < 1) num_columns = 1;

    // Allocate memory for column widths
    size_t *column_widths = calloc(num_columns, sizeof(size_t));
    if (!column_widths) {
        fprintf(stderr, "Memory allocation failed\n");
        free(entry_widths);
        return;
    }

    int rows = (num_entries + num_columns - 1) / num_columns;  // Calculate number of rows

    // Determine the width of each column
    for (int i = 0; i < num_entries; i++) {
        int col = i / rows;
        if (entry_widths[i] > column_widths[col]) {
            column_widths[col] = entry_widths[i];
        }
    }

    // Display entries in a grid
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < num_columns; col++) {
            int index = col * rows + row;
            if (index < num_entries) {
                printf("%s ", entries[index].emoji);  // Print emoji
                printf("%s", entries[index].name);    // Print name

                // Print Git status if applicable
                if (entries[index].git_status[0] != '\0') {
                    printf("\033[38;5;202m[%c]\033[0m", entries[index].git_status[0]);
                } else if (entries[index].is_directory && entries[index].contains_modified) {
                    printf("\033[38;5;202m[+]\033[0m");
                }

                // Add padding to align columns
                size_t printed_width = entry_widths[index];
                size_t padding = (col < num_columns - 1) ? column_widths[col] - printed_width + SPACING : 0;
                for (size_t i = 0; i < padding; i++) {
                    printf(" ");
                }
            }
        }
        printf("\n");  // Move to the next row
    }

    free(entry_widths);    // Free allocated memory
    free(column_widths);   // Free allocated memory
}
