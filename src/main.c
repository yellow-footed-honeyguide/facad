#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "file_entry.h"
#include "emoji_utils.h"
#include "display_utils.h"

#define MAX_PATH 4096

int main() {
    char current_dir[MAX_PATH];
    struct winsize w;
    int term_width;
    
    // Получаем текущую директорию
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd() error");
        return 1;
    }

    // Выводим текущую директорию жирным шрифтом
    printf("\033[1m%s\033[0m\n", current_dir);

    // Получаем ширину терминала
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    term_width = w.ws_col;

    // Открываем текущую директорию
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir() error");
        return 1;
    }

    // Создаем массив для хранения записей файлов
    FileEntry *entries = NULL;
    int num_entries = 0;
    struct dirent *entry;

    // Читаем содержимое директории
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        entries = realloc(entries, (num_entries + 1) * sizeof(FileEntry));
        if (entries == NULL) {
            perror("realloc() error");
            closedir(dir);
            return 1;
        }

        create_file_entry(&entries[num_entries], entry->d_name);
        num_entries++;
    }

    closedir(dir);

    // Сортируем записи
    qsort(entries, num_entries, sizeof(FileEntry), compare_file_entries);

    // Отображаем записи
    display_entries(entries, num_entries, term_width);

    // Освобождаем память
    for (int i = 0; i < num_entries; i++) {
        free(entries[i].name);
        free(entries[i].emoji);
    }
    free(entries);

    return 0;
}
