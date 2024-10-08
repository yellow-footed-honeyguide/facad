/**
 * @file dir_analytics.c
 * @brief Implementation of directory analytics functionality.
 *
 * This file contains functions for analyzing directory contents,
 * including calculation of total size, file counts, depth analysis,
 * and identification of largest and newest files.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "emoji_utils.h"
#include "dir_analytics.h"

#define MAX_PATH 4096
#define MAX_FILES 1024

// ANSI escape codes for text formatting
#define ANSI_BOLD "\033[1m"
#define ANSI_RESET "\033[0m"

/**
 * @brief Formats a file size into a human-readable string.
 *
 * @param size The size in bytes to format.
 * @return A pointer to a static buffer containing the formatted size string.
 */
static char* format_size(off_t size) {
    static char buf[64];
    const char* units[] = {"B", "K", "M", "G", "T", "P", "E", "Z", "Y"};
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
 * @brief Formats a time_t value into a human-readable string.
 *
 * @param t The time_t value to format.
 * @return A pointer to a static buffer containing the formatted time string.
 */
static char* format_time(time_t t) {
    static char buf[20];
    struct tm* tm_info = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return buf;
}

/**
 * @brief Recursively scans a directory to find its maximum depth.
 *
 * @param path The path of the directory to scan.
 * @param max_depth Pointer to the current maximum depth.
 * @param current_depth The current depth in the recursion.
 * @param deepest_dir Buffer to store the path of the deepest directory.
 */
static void recursive_dir_scan(const char* path, int* max_depth, int current_depth, char* deepest_dir) {
    DIR* dir;
    struct dirent* entry;
    char full_path[MAX_PATH];

    dir = opendir(path);
    if (!dir) return;

    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(full_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            int new_depth = current_depth + 1;
            // Update max_depth and deepest_dir if a new maximum is found
            if (new_depth > *max_depth) {
                *max_depth = new_depth;
                strncpy(deepest_dir, full_path, MAX_PATH - 1);
                deepest_dir[MAX_PATH - 1] = '\0';
            }
            // Recursively scan subdirectory
            recursive_dir_scan(full_path, max_depth, new_depth, deepest_dir);
        }
    }
    closedir(dir);
}

/**
 * @brief Calculates the total size of a directory and its contents.
 *
 * @param path The path of the directory to analyze.
 * @return The total size of the directory in bytes.
 */
static off_t calculate_total_size(const char* path) {
    DIR* dir;
    struct dirent* entry;
    struct stat st;
    off_t total_size = 0;
    char full_path[MAX_PATH];
    static long block_size = 0;

    // Determine the filesystem block size
    if (block_size == 0) {
        block_size = 512; // Default value
#ifdef ST_BLKSIZE
        struct statvfs buf;
        if (statvfs(path, &buf) == 0) {
            block_size = buf.f_frsize;
        }
#endif
    }

    dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "Error opening directory %s: %s\n", path, strerror(errno));
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (lstat(full_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // Recursively calculate size for subdirectories
                total_size += calculate_total_size(full_path);
            } else if (S_ISREG(st.st_mode)) {
                // Add file size, rounded up to the nearest block
                total_size += (st.st_blocks * 512 + block_size - 1) / block_size * block_size;
            }
            // Ignore symbolic links and other special files
        } else {
            fprintf(stderr, "Error stating %s: %s\n", full_path, strerror(errno));
        }
    }
    closedir(dir);
    return total_size;
}

/**
 * @brief Recursively finds the largest file in a directory and its subdirectories.
 *
 * @param path The path of the directory to scan.
 * @param largest_size Pointer to store the size of the largest file found.
 * @param largest_file Buffer to store the path of the largest file.
 * @param largest_file_size Size of the largest_file buffer.
 */
static void find_largest_file(const char* path, off_t* largest_size, char* largest_file, size_t largest_file_size) {
    DIR* dir;
    struct dirent* entry;
    char full_path[MAX_PATH];

    dir = opendir(path);
    if (!dir) return;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(full_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // Recursively scan subdirectories
                find_largest_file(full_path, largest_size, largest_file, largest_file_size);
            } else if (S_ISREG(st.st_mode)) {
                // Check if this file is larger than the current largest
                if (st.st_size > *largest_size) {
                    *largest_size = st.st_size;
                    strncpy(largest_file, full_path, largest_file_size - 1);
                    largest_file[largest_file_size - 1] = '\0';
                }
            }
        }
    }
    closedir(dir);
}


/**
 * @brief Prints detailed analytics about a directory.
 *
 * This function analyzes the contents of the specified directory
 * and prints various statistics including total size, file counts,
 * directory depth, and information about largest and newest files.
 *
 * @param path The path of the directory to analyze.
 */
void print_dir_analytics(const char* path) {
    DIR* dir;
    struct dirent* entry;
    struct stat st;
    off_t largest_size = 0;
    int total_files = 0, total_dirs = 0, empty_files = 0, max_depth = 0;
    time_t newest_time = 0, oldest_time = time(NULL);
    char largest_file[MAX_PATH] = "", newest_file[MAX_PATH] = "", oldest_file[MAX_PATH] = "";
    char empty_file_names[MAX_FILES][256];
    char deepest_dir[MAX_PATH] = "";

    dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    // Find the largest file recursively
    find_largest_file(path, &largest_size, largest_file, sizeof(largest_file));

    // Calculate total size recursively
    off_t total_size = calculate_total_size(path);

    while ((entry = readdir(dir)) != NULL) {
        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (lstat(full_path, &st) != 0) continue;

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if (S_ISDIR(st.st_mode)) {
            total_dirs++;
        } else if (S_ISREG(st.st_mode)) {
            total_files++;

            // Check for largest file
            if (st.st_size > largest_size) {
                largest_size = st.st_size;
                strncpy(largest_file, entry->d_name, sizeof(largest_file) - 1);
            }

            // Check for newest file
            if (st.st_mtime > newest_time) {
                newest_time = st.st_mtime;
                strncpy(newest_file, entry->d_name, sizeof(newest_file) - 1);
            }

            // Check for oldest file
            if (st.st_mtime < oldest_time) {
                oldest_time = st.st_mtime;
                strncpy(oldest_file, entry->d_name, sizeof(oldest_file) - 1);
            }

            // Check for empty files
            if (st.st_size == 0) {
                strncpy(empty_file_names[empty_files], entry->d_name, 255);
                empty_file_names[empty_files][255] = '\0';
                empty_files++;
            }
        }
    }
    closedir(dir);

    // Calculate max depth and find deepest directory
    recursive_dir_scan(path, &max_depth, 0, deepest_dir);

    char cwd[MAX_PATH];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    // Print analytics
    printf("%s%s%s\n", ANSI_BOLD, cwd, ANSI_RESET);
    printf("🧮 Total Size    : %s\n", format_size(total_size));
    printf("🗂️ Directories   : %d\n", total_dirs);
    printf("🌳 Max Depth     : %d levels\n", max_depth);
    printf("📁 Deepest Dir   : %s\n", deepest_dir);
    printf("🗃️ Files         : %d\n", total_files);
    printf("🐘 Largest File  : %s [%s]\n", largest_file, format_size(largest_size));
    printf("🏺 Oldest File   : %s [%s]\n", oldest_file, format_time(oldest_time));
    printf("🆕 Newest File   : %s [%s]\n", newest_file, format_time(newest_time));
    printf("📭 Empty Files   : %d [", empty_files);

    // Print names of empty files
    for (int i = 0; i < empty_files; i++) {
        printf("%s%s", empty_file_names[i], (i + 1 < empty_files) ? " " : "");
    }
    printf("]\n");
}
