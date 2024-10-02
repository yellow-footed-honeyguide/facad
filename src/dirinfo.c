#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include "dirinfo.h"
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

static off_t get_dir_size(const char *path) {
    DIR *dir;
    struct dirent *entry;
    char full_path[MAX_PATH];
    struct stat st;
    off_t total_size = 0;

    dir = opendir(path);
    if (!dir) return 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (lstat(full_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                total_size += get_dir_size(full_path);
            } else {
                total_size += st.st_size;
            }
        }
    }
    closedir(dir);
    return total_size;
}

void print_dirinfo(const char *path) {
    struct stat st;
    char full_path[MAX_PATH];
    DIR *dir;
    struct dirent *entry;
    int total_items = 0, files = 0, directories = 0;
    off_t total_size = 0, largest_size = 0;
    char largest_file[MAX_PATH] = "";
    int depth = 1, max_depth = 1;
    long long total_name_length = 0;
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

    printf("⚑ Directory Info\n");
    printf("  ╰─ Path      : %s\n", path);
    printf("  ╰─ Created   : %s", ctime(&st.st_ctime));
    printf("  ╰─ Modified  : %s", ctime(&st.st_mtime));
    printf("  ╰─ Owner     : %s\n", get_owner(st.st_uid, st.st_gid));
    char perms[11];
    get_permissions(st.st_mode, perms);
    printf("  ╰─ Perms     : %s\n", perms);

    dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (lstat(full_path, &st) != 0) continue;

        total_items++;
        total_name_length += strlen(entry->d_name);

        if (S_ISDIR(st.st_mode)) {
            directories++;
        } else {
            files++;
            if (st.st_size > largest_size) {
                largest_size = st.st_size;
                strncpy(largest_file, entry->d_name, sizeof(largest_file) - 1);
            }
            if (st.st_size == 0) empty_files++;
            if (st.st_size < min_size) min_size = st.st_size;
            if (st.st_size > max_size) max_size = st.st_size;
        }

        if (entry->d_name[0] == '.') hidden_items++;
        if (S_ISLNK(st.st_mode)) symlinks++;

        if (st.st_mtime > newest_time) {
            newest_time = st.st_mtime;
            strncpy(newest_file, entry->d_name, sizeof(newest_file) - 1);
        }
        if (st.st_mtime < oldest_time) {
            oldest_time = st.st_mtime;
            strncpy(oldest_file, entry->d_name, sizeof(oldest_file) - 1);
        }

        char *ext = strrchr(entry->d_name, '.');
        if (ext != NULL) {
            ext++; // Move past the dot
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

    total_size = get_dir_size(path);

    printf("  ╰─ Total Size: %s (including subdirectories)\n", format_size(total_size));

    printf("♦ Content Summary\n");
    printf("  ╰─ Total Items   : %d\n", total_items);
    printf("     ├─ Files      : %d\n", files);
    printf("     ╰─ Directories: %d\n", directories);

    printf("☼ Largest File\n");
    printf("  ╰─ Name : %s\n", largest_file);
    printf("  ╰─ Size : %s\n", format_size(largest_size));

    printf("⚐ Files and Directories\n");
    dir = opendir(path);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (lstat(full_path, &st) != 0) continue;

        char *emoji = get_emoji(full_path);
        printf("  ╰─ %s %s\n", emoji, entry->d_name);
        printf("     ├─ Type : %s\n", S_ISDIR(st.st_mode) ? "Directory" : "File");
        printf("     ├─ Size : %s\n", format_size(st.st_size));
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

    printf("🔬 Directory Analytics\n");
    printf("  ╰─ Depth          : %d levels (including root)\n", max_depth);
    printf("  ╰─ Avg Name Length: %.1f characters\n", (float)total_name_length / total_items);
    printf("  ╰─ Hidden Items   : %d\n", hidden_items);
    printf("  ╰─ Size Range     : %s - %s\n", format_size(min_size), format_size(max_size));
    printf("  ╰─ Median Size    : %s\n", format_size((min_size + max_size) / 2));
    printf("  ╰─ Newest File    : %s (%s)", newest_file, ctime(&newest_time));
    printf("  ╰─ Oldest File    : %s (%s)", oldest_file, ctime(&oldest_time));
    printf("  ╰─ File Extensions: %d unique (", unique_extensions);
    for (int i = 0; i < unique_extensions; i++) {
        printf("%s%s", i > 0 ? ", " : "", extensions[i]);
    }
    printf(")\n");
    printf("  ╰─ Symlinks       : %d\n", symlinks);
    printf("  ╰─ Empty Files    : %d\n", empty_files);
    printf("  ╰─ File/Dir Ratio : %.1f:1\n", (float)files / directories);
}
