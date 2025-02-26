/**
 * @file longlisting.c
 * @brief Implementation of detailed directory listing functionality.
 *
 * This file contains functions for generating and displaying a detailed
 * listing of directory contents, including file sizes, permissions,
 * modification times, and other attributes.
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
#include <pwd.h>
#include <grp.h>
#include <locale.h>
#include <errno.h>
#include <unistd.h>
#include <glob.h>
#include "longlisting.h"
#include "emoji_utils.h"

#define MAX_PATH 4096
#define MAX_ENTRIES 1000
#define MAX_TIME_AGO_LEN 30

/**
 * @struct file_info
 * @brief Structure to hold detailed information about a file or directory.
 */
struct file_info {
    char name[256];             /**< Name of the file or directory */
    char full_path[MAX_PATH];   /**< Full path to the file or directory */
    off_t size;                 /**< Size of the file or directory */
    mode_t mode;                /**< File mode (permissions) */
    time_t mtime;               /**< Last modification time */
    int is_dir;                 /**< Flag indicating if it's a directory */
    int subdir_count;           /**< Number of subdirectories (for directories) */
    char owner[256];            /**< Owner of the file or directory */
    char user_rights[16];       /**< User rights represented as emojis */
    char time_ago[MAX_TIME_AGO_LEN]; /**< Human-readable time since last modification */
};

// Function prototypes
static char *format_size(off_t size);
static void format_time_ago(time_t file_time, char *buf, size_t buf_size);
static void get_user_rights(mode_t mode, char *rights);
static off_t get_dir_size(const char *path);
static int count_subdirs(const char *path);
static int compare_entries(const void *a, const void *b);
static int get_file_info(const char *path, struct file_info *fi, size_t *max_owner_len, size_t *max_time_ago_len);
static void print_sorted_entries(struct file_info *entries, int entry_count, size_t max_owner_len, size_t max_time_ago_len);

/**
 * @brief Formats a file size into a human-readable string.
 *
 * @param size The size in bytes to format.
 * @return A pointer to a static buffer containing the formatted size string.
 */
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

/**
 * @brief Formats the time difference between now and a given time into a human-readable string.
 *
 * @param file_time The time to format.
 * @param buf Buffer to store the formatted string.
 * @param buf_size Size of the buffer.
 */
static void format_time_ago(time_t file_time, char *buf, size_t buf_size) {
    time_t now = time(NULL);
    double seconds = difftime(now, file_time);
    int minutes = (int)(seconds / 60);
    int hours = minutes / 60;
    int days = hours / 24;
    int months = days / 30;  // Approximation
    int years = days / 365;  // Approximation

    if (years > 0) {
        snprintf(buf, buf_size, "%dy %dm ago", years, months % 12);
    } else if (months > 0) {
        snprintf(buf, buf_size, "%dm %dd ago", months, days % 30);
    } else if (days > 0) {
        snprintf(buf, buf_size, "%dd ago", days);
    } else if (hours > 0) {
        snprintf(buf, buf_size, "%dh ago", hours);
    } else if (minutes > 0) {
        snprintf(buf, buf_size, "%dm ago", minutes);
    } else {
        snprintf(buf, buf_size, "just now");
    }
}

/**
 * @brief Converts file permissions to a string of emojis representing user rights.
 *
 * @param mode The file mode containing the permissions.
 * @param rights Buffer to store the emoji string.
 */
static void get_user_rights(mode_t mode, char *rights) {
    const char *read = "\xF0\x9F\x91\x80";           // 👀
    const char *write = "\xE2\x9C\x8F\xEF\xB8\x8F "; // ✏️
    const char *exec = "\xF0\x9F\x9A\x80";           // 🚀
    const char *no = "\xE2\x9D\x8C";                 // ❌

    strcpy(rights, "");
    strcat(rights, (mode & S_IRUSR) ? read : no);
    strcat(rights, (mode & S_IWUSR) ? write : no);
    strcat(rights, (mode & S_IXUSR) ? exec : no);
}

/**
 * @brief Calculates the total size of a directory and its contents.
 *
 * @param path The path of the directory.
 * @return The total size of the directory in bytes.
 */
static off_t get_dir_size(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    off_t total_size = 0;
    char full_path[MAX_PATH];

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

/**
 * @brief Counts the number of subdirectories in a given directory.
 *
 * @param path The path of the directory.
 * @return The number of subdirectories.
 */
static int count_subdirs(const char *path) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(path);
    if (!dir) return 0;

    while ((entry = readdir(dir)) != NULL) {
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
 * @brief Comparison function for sorting file_info structures.
 *
 * @param a Pointer to the first file_info structure.
 * @param b Pointer to the second file_info structure.
 * @return Integer less than, equal to, or greater than zero if a is found,
 *         respectively, to be less than, to match, or be greater than b.
 */
static int compare_entries(const void *a, const void *b) {
    const struct file_info *fa = (const struct file_info *)a;
    const struct file_info *fb = (const struct file_info *)b;

    if (fa->is_dir && !fb->is_dir) return -1;
    if (!fa->is_dir && fb->is_dir) return 1;

    if (fa->is_dir && fb->is_dir) {
        if (fa->subdir_count != fb->subdir_count) {
            return fb->subdir_count - fa->subdir_count;
        }
        if (fa->size != fb->size) {
            return (fa->size < fb->size) ? 1 : -1;
        }
    }

    if (!fa->is_dir && !fb->is_dir) {
        if (fa->size != fb->size) {
            return (fa->size < fb->size) ? 1 : -1;
        }
    }

    return strcasecmp(fa->name, fb->name);
}

/**
 * @brief Gets detailed information about a file.
 *
 * @param path The path to the file.
 * @param fi Pointer to a file_info structure to populate.
 * @param max_owner_len Pointer to store the maximum owner name length.
 * @param max_time_ago_len Pointer to store the maximum time ago string length.
 * @return 0 on success, -1 on failure.
 */
static int get_file_info(const char *path, struct file_info *fi, size_t *max_owner_len, size_t *max_time_ago_len) {
    struct stat st;
    if (lstat(path, &st) != 0) return -1;

    strncpy(fi->name, path, sizeof(fi->name) - 1);
    fi->name[sizeof(fi->name) - 1] = '\0';
    strncpy(fi->full_path, path, sizeof(fi->full_path) - 1);
    fi->full_path[sizeof(fi->full_path) - 1] = '\0';

    fi->mode = st.st_mode;
    fi->mtime = st.st_mtime;
    fi->is_dir = S_ISDIR(st.st_mode);
    fi->size = fi->is_dir ? get_dir_size(path) : st.st_size;
    fi->subdir_count = fi->is_dir ? count_subdirs(path) : 0;

    struct passwd *pw = getpwuid(st.st_uid);
    if (pw) {
        strncpy(fi->owner, pw->pw_name, sizeof(fi->owner) - 1);
    } else {
        snprintf(fi->owner, sizeof(fi->owner), "%d", st.st_uid);
    }

    size_t owner_len = strlen(fi->owner);
    if (owner_len > *max_owner_len) {
        *max_owner_len = owner_len;
    }

    format_time_ago(fi->mtime, fi->time_ago, sizeof(fi->time_ago));
    size_t time_ago_len = strlen(fi->time_ago);
    if (time_ago_len > *max_time_ago_len) {
        *max_time_ago_len = time_ago_len;
    }

    get_user_rights(fi->mode, fi->user_rights);

    return 0;
}

/**
 * @brief Prints sorted entries in a detailed format.
 *
 * @param entries Array of file_info structures.
 * @param entry_count Number of entries in the array.
 * @param max_owner_len Maximum length of owner names.
 * @param max_time_ago_len Maximum length of time ago strings.
 */
static void print_sorted_entries(struct file_info *entries, int entry_count, size_t max_owner_len, size_t max_time_ago_len) {
    for (int i = 0; i < entry_count; i++) {
        struct file_info *fi = &entries[i];
        char *emoji = get_emoji(fi->full_path);

        printf("%8s  %-*s  \xF0\x9F\x91\x91: %-*s \xF0\x9F\x93\x9C: %s  %s %-17s",
               format_size(fi->size),
               (int)max_time_ago_len, fi->time_ago,
               (int)max_owner_len, fi->owner,
               fi->user_rights,
               emoji,
               fi->name);

        if (fi->is_dir) {
            printf("(%d)", fi->subdir_count);
        }
        printf("\n");

        free(emoji);
    }
}

/**
 * @brief Prints a detailed listing of the contents of a directory.
 *
 * @param path The path of the directory to list.
 */
void print_longlisting(const char *path) {
    setlocale(LC_ALL, "");

    struct file_info entries[MAX_ENTRIES];
    int entry_count = 0;
    size_t max_owner_len = 0;
    size_t max_time_ago_len = 0;

    DIR *dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && entry_count < MAX_ENTRIES) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct file_info *fi = &entries[entry_count];
        if (get_file_info(full_path, fi, &max_owner_len, &max_time_ago_len) == 0) {
            entry_count++;
        }
    }
    closedir(dir);

    qsort(entries, entry_count, sizeof(struct file_info), compare_entries);
    print_sorted_entries(entries, entry_count, max_owner_len, max_time_ago_len);
}


void print_longlisting_files(const char **patterns, int pattern_count) {
    struct file_info entries[MAX_ENTRIES];
    int entry_count = 0;
    size_t max_owner_len = 0;
    size_t max_time_ago_len = 0;

    for (int i = 0; i < pattern_count; i++) {
        glob_t globbuf;
        int glob_result = glob(patterns[i], GLOB_TILDE, NULL, &globbuf);

        if (glob_result == 0) {
            for (size_t j = 0; j < globbuf.gl_pathc && entry_count < MAX_ENTRIES; j++) {
                struct file_info *fi = &entries[entry_count];
                if (get_file_info(globbuf.gl_pathv[j], fi, &max_owner_len, &max_time_ago_len) == 0) {
                    entry_count++;
                }
            }
        } else {
            fprintf(stderr, "glob error for pattern '%s': %s\n", patterns[i], strerror(errno));
        }
        globfree(&globbuf);
    }

    if (entry_count > 0) {
        qsort(entries, entry_count, sizeof(struct file_info), compare_entries);
        print_sorted_entries(entries, entry_count, max_owner_len, max_time_ago_len);
    } else {
        fprintf(stderr, "No matching files found.\n");
    }
}
