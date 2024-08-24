#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include "display_utils.h"
#include "file_entry.h"

#define MAX_COLUMNS 4

int get_display_width(const char* str) {
    setlocale(LC_ALL, "");
    wchar_t* wstr = (wchar_t*)malloc((strlen(str) + 1) * sizeof(wchar_t));
    if (wstr == NULL) {
        return 0;
    }
    mbstowcs(wstr, str, strlen(str) + 1);
    int width = wcswidth(wstr, wcslen(wstr));
    free(wstr);
    return width;
}

void display_entries(FileEntry* entries, int num_entries, int term_width) {
    int num_columns = 1;
    int* column_widths = NULL;

    // Находим оптимальное количество столбцов
    for (int cols = MAX_COLUMNS; cols >= 1; cols--) {
        int rows = (num_entries + cols - 1) / cols;
        column_widths = (int*)malloc(cols * sizeof(int));
        if (column_widths == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            return;
        }

        for (int col = 0; col < cols; col++) {
            column_widths[col] = 0;
            for (int row = 0; row < rows && col * rows + row < num_entries; row++) {
                int index = col * rows + row;
                int width = get_display_width(entries[index].emoji) + 1 + 
                            get_display_width(entries[index].name);
                if (width > column_widths[col]) {
                    column_widths[col] = width;
                }
            }
        }

        int total_width = 0;
        for (int col = 0; col < cols; col++) {
            total_width += column_widths[col];
        }
        total_width += (cols - 1) * 2; // 2 пробела между столбцами

        if (total_width <= term_width) {
            num_columns = cols;
            break;
        }

        free(column_widths);
        column_widths = NULL;
    }

    if (column_widths == NULL) {
        fprintf(stderr, "Unable to determine column layout\n");
        return;
    }

    int rows = (num_entries + num_columns - 1) / num_columns;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < num_columns; col++) {
            int index = row + col * rows;
            if (index < num_entries) {
                int display_width = get_display_width(entries[index].emoji) + 1 + 
                                    get_display_width(entries[index].name);
                printf("%s %s", entries[index].emoji, entries[index].name);

                // Добавляем пробелы для выравнивания в пределах столбца
                if (col < num_columns - 1) {
                    for (int i = 0; i < column_widths[col] - display_width; i++) {
                        printf(" ");
                    }
                    printf("  "); // 2 пробела между столбцами
                }
            }
        }
        printf("\n");
    }

    free(column_widths);
}
