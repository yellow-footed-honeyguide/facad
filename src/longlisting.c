#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include "longlisting.h"
#include "emoji_utils.h"

#define MAX_PATH 4096

static char *format_size(off_t size) {
    static char buf[64];
    const char *units[] = {"B", "K", "M", "G", "T", "P", "E", "Z", "Y"};
    int i = 0;
    double dsize = size;

    while (dsize >= 1024 && i < 8) {
        dsize /= 1024;
        i++;
    }

    snprintf(buf, sizeof(buf), "%.1f%s", dsize, units[i]);
    return buf;
}

static void get_permissions(mode_t mode, char *perms) {
    snprintf(perms, 11, "%c%c%c%c%c%c%c%c%c%c",
        (S_ISDIR(mode)) ? 'd' : '-',
        (mode & S_IRUSR) ? 'r' : '-',
        (mode & S_IWUSR) ? 'w' : '-',
        (mode & S_IXUSR) ? 'x' : '-',
        (mode & S_IRGRP) ? 'r' : '-',
        (mode & S_IWGRP) ? 'w' : '-',
        (mode & S_IXGRP) ? 'x' : '-',
        (mode & S_IROTH) ? 'r' : '-',
        (mode & S_IWOTH) ? 'w' : '-',
        (mode & S_IXOTH) ? 'x' : '-'
    );
}

void print_longlisting(const char *path) {
    DIR *dir;
    struct dirent *entry;
    char full_path[MAX_PATH];
    struct stat st;

    dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    printf("🗂️ Files and Directories\n");
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (lstat(full_path, &st) != 0) continue;

        char *emoji = get_emoji(full_path);
        printf("  ╰─ %s %s\n", emoji, entry->d_name);
        printf("     ├─ Type : %s\n", S_ISDIR(st.st_mode) ? "Directory" : "File");
        printf("     ├─ Size : %s\n", format_size(st.st_size));
        char perms[11];
        get_permissions(st.st_mode, perms);
        printf("     ╰─ Perms: %s\n", perms);
        if (S_ISDIR(st.st_mode)) {
            int subdir_count = 0;
            DIR *subdir = opendir(full_path);
            if (subdir) {
                struct dirent *subentry;
                while ((subentry = readdir(subdir)) != NULL) {
                    char sub_full_path[MAX_PATH];
                    snprintf(sub_full_path, sizeof(sub_full_path), "%s/%s", full_path, subentry->d_name);
                    struct stat sub_st;
                    if (lstat(sub_full_path, &sub_st) == 0 && S_ISDIR(sub_st.st_mode) &&
                        strcmp(subentry->d_name, ".") != 0 && strcmp(subentry->d_name, "..") != 0) {
                        subdir_count++;
                    }
                }
                closedir(subdir);
            }
            printf("     ╰─ Subdirs: %d\n", subdir_count);
        }
        free(emoji);
    }
    closedir(dir);
}
