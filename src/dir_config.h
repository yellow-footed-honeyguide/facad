/**
 * @file dir_config.h
 * @brief Header file for directory-specific configurations.
 *
 * This file declares structures and functions for managing
 * directory-specific configurations, particularly for customizing
 * the display of directory listings.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef DIRECTORY_CONFIG_H
#define DIRECTORY_CONFIG_H

/**
 * @brief Default maximum number of columns for directory listings.
 *
 * This value is used when no specific configuration is found for a directory.
 */
#define DEFAULT_MAX_COLUMNS 4

/**
 * @struct DirectoryConfig
 * @brief Structure to hold directory-specific configuration.
 *
 * This structure associates a directory path with its custom
 * maximum number of columns for listing display.
 */
typedef struct {
    const char *path;      /**< Path of the directory */
    int max_columns;       /**< Maximum number of columns for listing */
} DirectoryConfig;

/**
 * @brief Array of directory configurations.
 *
 * This extern declaration makes the array of DirectoryConfig structures
 * available to other source files that include this header.
 */
extern const DirectoryConfig directory_configs[];

/**
 * @brief Number of entries in the directory_configs array.
 *
 * This extern declaration makes the count of directory configurations
 * available to other source files that include this header.
 */
extern const int num_directory_configs;

/**
 * @brief Get the maximum number of columns for a given directory path.
 *
 * This function determines the maximum number of columns to use when
 * displaying the contents of the specified directory. It checks for
 * custom configurations and returns the default if none is found.
 *
 * @param path The path of the directory to check.
 * @return The maximum number of columns for the directory listing.
 */
int get_max_columns(const char *path);

#endif // DIRECTORY_CONFIG_H
