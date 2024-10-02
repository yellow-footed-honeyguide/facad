#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <errno.h>
#include "dir_analytics.h"
#include "emoji_utils.h"

#define MAX_PATH 4096
#define MAX_FILES 1024

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

static char *get_owner(uid_t uid, gid_t gid) {
    static char owner[256];
    struct passwd *pw = getpwuid(uid);
    struct group *gr = getgrgid(gid);

    if (pw && gr) {
        snprintf(owner, sizeof(owner), "%s:%s", pw->pw_name, gr->gr_name);
    } else {
        snprintf(owner, sizeof(owner), "%d:%d", uid, gid);
    }
    return owner;
}

static void recursive_dir_scan(const char *path, off_t *total_size, int *total_dirs, int *total_files, off_t *min_size, off_t *max_size, time_t *newest_time, time_t *oldest_time, char *newest_file, char *oldest_file) {
    DIR *dir;
    struct dirent *entry;
    char full_path[MAX_PATH];
    struct stat st;

    dir = opendir(path);
    if (!dir) return;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (lstat(full_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                (*total_dirs)++;
                recursive_dir_scan(full_path, total_size, total_dirs, total_files, min_size, max_size, newest_time, oldest_time, newest_file, oldest_file);
            } else {
                (*total_files)++;
                *total_size += st.st_size;

                if (st.st_size < *min_size) *min_size = st.st_size;
                if (st.st_size > *max_size) *max_size = st.st_size;

                if (st.st_mtime > *newest_time) {
                    *newest_time = st.st_mtime;
                    strncpy(newest_file, full_path, MAX_PATH - 1);
                    newest_file[MAX_PATH - 1] = '\0';
                }
                if (st.st_mtime < *oldest_time) {
                    *oldest_time = st.st_mtime;
                    strncpy(oldest_file, full_path, MAX_PATH - 1);
                    oldest_file[MAX_PATH - 1] = '\0';
                }
            }
        }
    }
    closedir(dir);
}

static int get_dir_depth(const char *path) {
    DIR *dir;
    struct dirent *entry;
    char full_path[MAX_PATH];
    int max_depth = 0;

    dir = opendir(path);
    if (!dir) return 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            int depth = get_dir_depth(full_path);
            if (depth > max_depth) max_depth = depth;
        }
    }
    closedir(dir);
    return max_depth + 1;
}

static char *format_time(time_t t) {
    static char buf[20];
    struct tm *tm_info = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return buf;
}

void print_dir_analytics(const char *path) {
    struct stat st;
    DIR *dir;
    struct dirent *entry;
    int total_items = 0, files = 0, directories = 0, total_dirs = 0;
    off_t total_size = 0, largest_size = 0;
    char largest_file[MAX_PATH] = "";
    int max_depth = 0;
    int hidden_items = 0;
    off_t min_size = LLONG_MAX, max_size = 0;
    time_t newest_time = 0, oldest_time = time(NULL);
    char newest_file[MAX_PATH] = "", oldest_file[MAX_PATH] = "";
    int symlinks = 0, empty_files = 0;
    char extensions[MAX_FILES][32];
    int unique_extensions = 0;

    if (lstat(path, &st) != 0) {
        perror("Error getting directory info");
        return;
    }

    recursive_dir_scan(path, &total_size, &total_dirs, &files, &min_size, &max_size, &newest_time, &oldest_time, newest_file, oldest_file);
    max_depth = get_dir_depth(path);

    dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (lstat(full_path, &st) != 0) continue;

        total_items++;

        if (S_ISDIR(st.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                directories++;
            }
        } else {
            if (st.st_size > largest_size) {
                largest_size = st.st_size;
                strncpy(largest_file, entry->d_name, sizeof(largest_file) - 1);
            }
            if (st.st_size == 0) empty_files++;
        }

        if (entry->d_name[0] == '.') hidden_items++;
        if (S_ISLNK(st.st_mode)) symlinks++;

        char *ext = strrchr(entry->d_name, '.');
        if (ext != NULL) {
            ext++;
            int found = 0;
            for (int i = 0; i < unique_extensions; i++) {
                if (strcmp(extensions[i], ext) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found && unique_extensions < MAX_FILES) {
                strncpy(extensions[unique_extensions], ext, sizeof(extensions[0]) - 1);
                unique_extensions++;
            }
        }
    }
    closedir(dir);

    char perms[11];
    get_permissions(st.st_mode, perms);

    printf("🔬 Directory Analytics\n");
    printf("  ╰─🧭 Path         : %s\n", path);
    printf("  ╰─🎂 Created      : %s\n", format_time(st.st_ctime));
    printf("  ╰─✏️  Modified     : %s\n", format_time(st.st_mtime));
    printf("  ╰─👤 Owner        : %s\n", get_owner(st.st_uid, st.st_gid));
    printf("  ╰─🚦 Perms        : %s\n", perms);
    printf("  ╰─🧮 Total Size   : %s (including subdirs)\n", format_size(total_size));
    printf("  ╰─🧮 Total Items  : %d\n", total_items);
    printf("  ╰─🗃️ Files        : %d\n", files);
    printf("  ╰─🗂️ Dirs         : %d (current directory)\n", directories);
    printf("  ╰─🗂️ Total Dirs   : %d (including subdirs)\n", total_dirs);
    printf("  ╰─🌳 Depth        : %d levels\n", max_depth);
    printf("  ╰─🕵️ Hidden Items : %d\n", hidden_items);
    printf("  ╰─🐘 Largest File : %s [%s]\n", largest_file, format_size(largest_size));
    printf("  ╰─📏 Size Range   : %s - %s\n", format_size(min_size), format_size(max_size));
    printf("  ╰─⚖️  Median Size  : %s\n", format_size((min_size + max_size) / 2));
    printf("  ╰─🆕 Newest File  : %s [%s]\n", newest_file, format_time(newest_time));
    printf("  ╰─🏺 Oldest File  : %s [%s]\n", oldest_file, format_time(oldest_time));
    printf("  ╰─🌉 Symlinks     : %d\n", symlinks);
    printf("  ╰─📭 Empty Files  : %d\n", empty_files);
    printf("  ╰─🌓 Ratio        : %.1f files/1 dir\n", total_dirs > 0 ? (float)files / total_dirs : 0);
    printf("  ╰─🧩 Extensions   : %d unique [", unique_extensions);
    for (int i = 0; i < unique_extensions; i++) {
        printf("%s%s", i > 0 ? ", " : "", extensions[i]);
    }
    printf("]\n");
}
