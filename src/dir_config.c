/**
 * @file dir_config.c
 * @brief Implementation of directory configuration functions.
 *
 * This file contains the implementation of functions related to
 * directory-specific configurations, particularly for setting
 * the maximum number of columns for directory listings.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include "dir_config.h"
#include <string.h>

/**
 * @brief Array of directory configurations.
 *
 * This array defines special configurations for specific directories,
 * setting a custom maximum number of columns for their listings.
 */
const DirectoryConfig directory_configs[] = {
    {"/dev", 6},  // Set 6 columns for /dev directory
    {"/proc", 5}, // Set 5 columns for /proc directory
};

/**
 * @brief Number of entries in the directory_configs array.
 */
const int num_directory_configs = sizeof(directory_configs) / sizeof(directory_configs[0]);

/**
 * @brief Get the maximum number of columns for a given directory path.
 *
 * This function checks if the given path matches any of the specially
 * configured directories and returns the corresponding maximum number
 * of columns. If no match is found, it returns the default value.
 *
 * @param path The directory path to check.
 * @return The maximum number of columns for the directory listing.
 */
int get_max_columns(const char *path) {
    // Iterate through the directory configurations
    for (int i = 0; i < num_directory_configs; i++) {
        // Check if the path matches a configured directory
        if (strcmp(path, directory_configs[i].path) == 0) {
            // Return the custom max columns for the matched directory
            return directory_configs[i].max_columns;
        }
    }

    // If no match found, return the default maximum columns
    return DEFAULT_MAX_COLUMNS;
}
