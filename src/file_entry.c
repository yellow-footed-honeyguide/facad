#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "file_entry.h"
#include "emoji_utils.h"

int create_file_entry(FileEntry* entry, const char* path) {
    entry->name = strdup(path);
    entry->emoji = get_emoji(path);

    struct stat path_stat;
    if (lstat(path, &path_stat) != 0) {
        return -1;  // Возвращаем ошибку, если не удалось получить информацию о файле
    }

    entry->is_directory = S_ISDIR(path_stat.st_mode);
    entry->is_hidden = (path[0] == '.');

    return 0;  // Успешное создание записи
}

void free_file_entry(FileEntry* entry) {
    free(entry->name);
    free(entry->emoji);
}

char* get_extension(const char* name) {
    char* dot = strrchr(name, '.');
    if (!dot || dot == name) {
        return "";
    }
    return dot + 1;
}

int compare_file_entries(const void* a, const void* b) {
    const FileEntry* entry_a = (const FileEntry*)a;
    const FileEntry* entry_b = (const FileEntry*)b;

    // Сначала сортируем директории
    if (entry_a->is_directory && !entry_b->is_directory) {
        return -1;
    }
    if (!entry_a->is_directory && entry_b->is_directory) {
        return 1;
    }

    // Затем сортируем скрытые файлы и директории
    if (entry_a->is_hidden && !entry_b->is_hidden) {
        return -1;
    }
    if (!entry_a->is_hidden && entry_b->is_hidden) {
        return 1;
    }

    // Наконец, сортируем по имени (без учета регистра)
    return strcasecmp(entry_a->name, entry_b->name);
}
