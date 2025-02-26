/**
 * @file emoji_utils.c
 * @brief Implementation of emoji utility functions for file representation
 *
 * This file contains functions for determining appropriate emojis
 * for files based on their types, attributes, and locations. It supports
 * both regular files and special files in the /dev directory.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "emoji_utils.h"
#include "emoji_mappings.h"

#define MAX_PATH 4096

/**
 * @brief Safely duplicates a string, handling memory allocation errors
 *
 * @param str The string to duplicate
 * @return A newly allocated copy of the input string
 */
static char *safe_strdup(const char *str) {
    char *dup = strdup(str);
    if (!dup) {
        perror("strdup"); // Print error message if strdup fails
        exit(EXIT_FAILURE); // Exit the program on allocation failure
    }
    return dup;
}

/**
 * @brief Determines the appropriate emoji for files in the /dev directory
 *
 * @param path The name of the file in the /dev directory
 * @return A dynamically allocated string containing the emoji
 */
char *get_dev_emoji(const char *path) {
    // Check for prefix matches in the dev prefix map
    for (size_t i = 0; i < emoji_prefix_dev_map_size; i++) {

        if (strstr(path, "ttyACM") != NULL) {
            return safe_strdup("♾️ ");
        }

        if (strncmp(path, emoji_prefix_dev_map[i].key, strlen(emoji_prefix_dev_map[i].key)) == 0) {
            return safe_strdup(emoji_prefix_dev_map[i].emoji);
        }
    }

    // Check for exact matches in the dev file map
    for (size_t i = 0; i < sizeof(emoji_exact_dev_file_map_size); i++) {
        if (strcmp(path, emoji_exact_dev_file_map[i].key) == 0) {
            return safe_strdup(emoji_exact_dev_file_map[i].emoji);
        }
    }

    // Return default emoji for unmatched /dev files
    return safe_strdup("🔧");
}

/**
 * @brief Checks the file content for specific patterns to determine the file type
 *
 * @param path The path to the file
 * @return A dynamically allocated string containing the emoji, or NULL if no match
 */
static char* check_file_content(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) return NULL;

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Check each entry in the file content map
        for (size_t i = 0; i < emoji_file_content_map_size; i++) {
            if (strstr(buffer, emoji_file_content_map[i].key) != NULL) {
                fclose(file);
                return safe_strdup(emoji_file_content_map[i].emoji);
            }
        }
    }

    fclose(file);
    return NULL;
}

/**
 * @brief Determines the appropriate emoji for a given file based on its characteristics
 *
 * @param path The path to the file
 * @return A dynamically allocated string containing the emoji
 */
char *get_emoji(const char *path) {
   struct stat path_stat;

    // Check if we can get file information
    if (lstat(path, &path_stat) != 0) {
        return safe_strdup("❓");  // Unknown file type
    }

    // Check for symbolic links
    if (S_ISLNK(path_stat.st_mode)) {
        return safe_strdup(S_ISDIR(path_stat.st_mode) ? "🔗📁" : "🔗");
    }

    // Check for directories
    if (S_ISDIR(path_stat.st_mode)) {
        return safe_strdup("📁");
    }

    // Extract the filename from the path
    const char *filename = strrchr(path, '/');
    filename = filename ? filename + 1 : path;

    // Check for not-exact file name matches first (top highest priority)
    for (size_t i = 0; i < emoji_not_exact_file_map_size; i++) {
        if (strstr(filename, emoji_not_exact_file_map[i].key) != NULL) {
            return safe_strdup(emoji_not_exact_file_map[i].emoji);
        }
    }

    // Check for exact file name matches first (highest priority)
    for (size_t i = 0; i < emoji_exact_file_map_size; i++) {
        if (strcasecmp(filename, emoji_exact_file_map[i].key) == 0) {
            return safe_strdup(emoji_exact_file_map[i].emoji);
        }
    }

    // Check file content for specific patterns (second priority)
    char *content_emoji = check_file_content(path);
    if (content_emoji) {
        return content_emoji;
    }

    // Check file extensions (third priority)
    char *extension = strrchr(filename, '.');
    if (extension) {
        extension++;  // Skip the dot
        for (size_t i = 0; i < emoji_extension_map_size; i++) {
            if (strcasecmp(extension, emoji_extension_map[i].key) == 0) {
                return safe_strdup(emoji_extension_map[i].emoji);
            }
        }
    }

    // Check for hidden files
    if (filename[0] == '.') {
        return safe_strdup("⚙️ ");
    }

    // Check for executable files
    if (is_executable(path)) {
        return safe_strdup("💾");
    }

    // Check for text files
    if (is_text_file(path)) {
        return safe_strdup("📝");
    }

    // Default emoji for unclassified files
    return safe_strdup("❓");
}

/**
 * @brief Checks if a file is executable
 *
 * @param path The path to the file
 * @return 1 if the file is executable, 0 otherwise
 */
int is_executable(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return (st.st_mode & S_IXUSR) != 0; // Check if the user execute bit is set
    }
    return 0;
}

/**
 * @brief Checks if a file is a text file by examining its contents
 *
 * @param path The path to the file
 * @return 1 if the file is likely a text file, 0 otherwise
 */
int is_text_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        return 0; // Cannot open file, assume it's not text
    }

    unsigned char buffer[1024];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    if (bytesRead == 0) {
        return 1;  // Empty file is considered text
    }

    // Check if all read bytes are printable or whitespace
    for (size_t i = 0; i < bytesRead; i++) {
        if (!isprint(buffer[i]) && !isspace(buffer[i])) {
            return 0; // Non-printable, non-whitespace character found
        }
    }
    return 1; // All characters are printable or whitespace
}
