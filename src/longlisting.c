/**
 * @file longlisting.c
 * @brief Implements detailed directory listing functionality
 *
 * This file contains functions for generating and displaying
 * a detailed listing of directory contents, including file sizes,
 * permissions, modification times, and other attributes.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <locale.h>
#include "longlisting.h"
#include "emoji_utils.h"

#define MAX_PATH 4096
#define MAX_ENTRIES 1000

/**
 * @brief Structure to hold file information for sorting and display
 */
struct file_info {
    char name[256];         /**< File name */
    char full_path[MAX_PATH]; /**< Full path to the file */
    off_t size;             /**< File size in bytes */
    mode_t mode;            /**< File mode (permissions) */
    time_t mtime;           /**< Last modification time */
    int is_dir;             /**< Flag indicating if it's a directory */
    int subdir_count;       /**< Number of subdirectories (if it's a directory) */
};

/**
 * @brief Formats a file size into a human-readable string
 *
 * @param size The size in bytes to format
 * @return A static string containing the formatted size
 */
static char *format_size(off_t size) {
    static char buf[64];
    const char *units[] = {"B", "K", "M", "G", "T", "P", "E", "Z", "Y"};
    int i = 0;
    double dsize = size;

    // Convert size to appropriate unit
    while (dsize >= 1024 && i < 8) {
        dsize /= 1024;
        i++;
    }

    // Format the size with one decimal place
    snprintf(buf, sizeof(buf), "%.1f%s", dsize, units[i]);
    return buf;
}

/**
 * @brief Converts file permissions to a human-readable string with emojis
 *
 * @param mode The file mode containing the permissions
 * @return A static string containing the formatted permissions
 */
static char *get_human_readable_perms(mode_t mode) {
    static char perms[64];  // Increased buffer size
    char *p = perms;

    // Format permissions using emojis
    p += sprintf(p, "👤 %s%s%s 👥 %s%s%s 🌍 %s%s%s",
        (mode & S_IRUSR) ? "👀" : "❌",
        (mode & S_IWUSR) ? "✏️" : "❌",
        (mode & S_IXUSR) ? "🚀" : "❌",
        (mode & S_IRGRP) ? "👀" : "❌",
        (mode & S_IWGRP) ? "✏️" : "❌",
        (mode & S_IXGRP) ? "🚀" : "❌",
        (mode & S_IROTH) ? "👀" : "❌",
        (mode & S_IWOTH) ? "✏️" : "❌",
        (mode & S_IXOTH) ? "🚀" : "❌");

    // Add special permission flags
    if (mode & S_ISUID) p += sprintf(p, "🔑");
    if (mode & S_ISGID) p += sprintf(p, "🔐");
    if (mode & S_ISVTX) p += sprintf(p, "🔒");

    return perms;
}

/**
 * @brief Counts the number of subdirectories in a given directory
 *
 * @param path The path to the directory
 * @return The number of subdirectories
 */
static int count_subdirs(const char *path) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(path);
    if (!dir) return 0;

    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            count++;
        }
    }
    closedir(dir);
    return count;
}

/**
 * @brief Comparison function for sorting file entries
 *
 * Sorts entries based on whether they're directories, then by extension,
 * then by size, and finally by name.
 *
 * @param a Pointer to the first file_info struct
 * @param b Pointer to the second file_info struct
 * @return Integer less than, equal to, or greater than zero if a is found,
 *         respectively, to be less than, to match, or be greater than b
 */
static int compare_entries(const void *a, const void *b) {
    const struct file_info *fa = (const struct file_info *)a;
    const struct file_info *fb = (const struct file_info *)b;

    // Sort directories before files
    if (fa->is_dir && !fb->is_dir) return -1;
    if (!fa->is_dir && fb->is_dir) return 1;

    // For directories, sort by number of subdirectories
    if (fa->is_dir && fb->is_dir) {
        return fb->subdir_count - fa->subdir_count;
    }

    // Sort by file extension
    char *ext_a = strrchr(fa->name, '.');
    char *ext_b = strrchr(fb->name, '.');

    if (ext_a && ext_b) {
        int cmp = strcasecmp(ext_a, ext_b);
        if (cmp != 0) return cmp;
    } else if (ext_a) {
        return 1;
    } else if (ext_b) {
        return -1;
    }

    // Sort by size (larger files first)
    if (fa->size < fb->size) return 1;
    if (fa->size > fb->size) return -1;

    // Finally, sort alphabetically
    return strcasecmp(fa->name, fb->name);
}

/**
 * @brief Prints a detailed listing of the contents of a directory
 *
 * This function reads the contents of the specified directory,
 * sorts them, and prints a detailed listing including file sizes,
 * modification times, permissions, and other attributes.
 *
 * @param path The path of the directory to list
 */
void print_longlisting(const char *path) {
    setlocale(LC_ALL, "");  // Set locale to support UTF-8

    DIR *dir;
    struct dirent *entry;
    struct file_info entries[MAX_ENTRIES];
    int entry_count = 0;

    dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    // Read directory entries
    while ((entry = readdir(dir)) != NULL && entry_count < MAX_ENTRIES) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        struct file_info *fi = &entries[entry_count];
        strncpy(fi->name, entry->d_name, sizeof(fi->name) - 1);
        snprintf(fi->full_path, sizeof(fi->full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(fi->full_path, &st) != 0) continue;

        fi->size = st.st_size;
        fi->mode = st.st_mode;
        fi->mtime = st.st_mtime;
        fi->is_dir = S_ISDIR(st.st_mode);
        fi->subdir_count = fi->is_dir ? count_subdirs(fi->full_path) : 0;

        entry_count++;
    }
    closedir(dir);

    // Sort the entries
    qsort(entries, entry_count, sizeof(struct file_info), compare_entries);

    // Print sorted entries
    for (int i = 0; i < entry_count; i++) {
        struct file_info *fi = &entries[i];
        char *emoji = get_emoji(fi->full_path);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", localtime(&fi->mtime));

        char *perms = get_human_readable_perms(fi->mode);

        printf("%8s  %s  %-40s %-2s %-20s",
               format_size(fi->size),
               time_str,
               perms,
               emoji,
               fi->name);

        if (fi->is_dir) {
            printf(" 📁 (%d subdirs)", fi->subdir_count);
        }
        printf("\n");

        free(emoji);
    }
}
