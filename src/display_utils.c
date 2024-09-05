#define _XOPEN_SOURCE 700

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h> // For using wcwidth

#include "display_utils.h"
#include "file_entry.h"

#define MAX_COLUMNS 4
#define SPACING 2

// Function to determine the width of a UTF-8 character
size_t get_utf8_char_width(const char *str)
{
    unsigned char c = (unsigned char)*str;
    if (c < 0x80)
        return 1; // ASCII characters
    if (c < 0xE0)
        return 2; // 2-byte UTF-8 characters
    if (c < 0xF0)
        return 3; // 3-byte UTF-8 characters
    return 4;     // 4-byte UTF-8 characters (including emoji)
}

// Function to calculate the display width of a string
size_t get_display_width(const char *str)
{
    size_t width = 0;
    wchar_t wc;
    int len;

    // Iterate through each character in the string
    while (*str)
    {
        len = mbtowc(&wc, str, MB_CUR_MAX); // Convert multi-byte character to wide character
        if (len > 0)
        {
            width += wcwidth(wc); // Get the display width of the wide character
            str += len;           // Move to the next character
        }
        else
        {
            str += 2; // Move to the next character if conversion fails
        }
    }

    return width;
}

// Function to display entries in a grid format
void display_entries(FileEntry *entries, int num_entries, int term_width)
{
    setlocale(LC_ALL, ""); // Set locale to the user's environment

    qsort(entries, num_entries, sizeof(FileEntry), compare_file_entries); // Sort entries based on the comparison function

    size_t *entry_widths = malloc(num_entries * sizeof(size_t)); // Allocate memory for storing the widths of each entry
    if (!entry_widths)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    size_t max_width = 0;
    for (int i = 0; i < num_entries; i++)
    {
        entry_widths[i] = get_display_width(entries[i].emoji) + get_display_width(entries[i].name); // Calculate the total width of each entry (emoji + name)
        if (entry_widths[i] > max_width)
        {
            max_width = entry_widths[i]; // Track the maximum width
        }
    }

    int num_columns = (term_width + SPACING) / (max_width + SPACING); // Determine the number of columns based on terminal width
    if (num_columns > MAX_COLUMNS)
        num_columns = MAX_COLUMNS; // Limit the number of columns to MAX_COLUMNS
    if (num_columns < 1)
        num_columns = 1; // Ensure at least one column

    int rows = (num_entries + num_columns - 1) / num_columns; // Calculate the number of rows needed

    size_t *column_widths = calloc(num_columns, sizeof(size_t)); // Allocate memory for storing the widths of each column
    if (!column_widths)
    {
        fprintf(stderr, "Memory allocation failed\n");
        free(entry_widths);
        return;
    }

    // Calculate the maximum width for each column
    for (int col = 0; col < num_columns; col++)
    {
        size_t column_max_width = 0;
        for (int row = 0; row < rows; row++)
        {
            int index = col * rows + row;
            if (index < num_entries)
            {
                if (entry_widths[index] > column_max_width)
                {
                    column_max_width = entry_widths[index];
                }
            }
        }
        column_widths[col] = column_max_width; // Store the maximum width for the column
    }

    // Print each row of entries
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < num_columns; col++)
        {
            int index = col * rows + row;
            if (index < num_entries)
            {
                printf("%s%s", entries[index].emoji, entries[index].name); // Print the emoji followed by the file name

                size_t padding = column_widths[col] - get_display_width(entries[index].emoji) - get_display_width(entries[index].name); // Calculate the padding needed
                for (size_t i = 0; i < padding; i++)
                {
                    printf(" "); // Print the padding
                }

                if (col < num_columns - 1)
                {
                    printf("%*s", SPACING, ""); // Print the spacing between columns
                }
            }
        }
        printf("\n"); // Move to the next line after printing all columns in the current row
    }

    free(entry_widths);  // Free the allocated memory for entry widths
    free(column_widths); // Free the allocated memory for column widths
}
