#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "dev_utils.h"
#include "emoji_utils.h"

#define MAX_PATH 4096

int is_dev_directory(const char *path) {
    return strcmp(path, "/dev") == 0;
}

void handle_dev_directory(FileEntry **entries, int *num_entries, int max_entries) {
    DIR *dir = opendir("/dev");
    if (!dir) {
        perror("Error opening /dev");
        return;
    }

    struct dirent *d_entry;
    char full_path[MAX_PATH];
    struct stat st;

    while ((d_entry = readdir(dir)) != NULL && *num_entries < max_entries) {
        if (strcmp(d_entry->d_name, ".") == 0 || strcmp(d_entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "/dev/%s", d_entry->d_name);

        if (lstat(full_path, &st) == 0) {
            FileEntry *entry = &((*entries)[*num_entries]);
            entry->name = strdup(d_entry->d_name);
            entry->is_directory = S_ISDIR(st.st_mode);
            entry->is_hidden = (d_entry->d_name[0] == '.');

            if (S_ISLNK(st.st_mode)) {
                entry->emoji = strdup(entry->is_directory ? "🔗📁" : "🔗");
            } else if (entry->is_directory) {
                entry->emoji = strdup("📁");
            } else {
                entry->emoji = get_dev_emoji(d_entry->d_name);
            }

            (*num_entries)++;
        }
    }

    closedir(dir);
}
