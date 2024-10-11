/**
 * @file dev_dir_utils.c
 * @brief Utility functions for handling the /dev directory
 *
 * This file contains functions to identify and process entries in the /dev
 * directory, which contains device files on Unix-like systems.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#include "dev_dir_utils.h"
#include "emoji_utils.h"

#define MAX_PATH 4096 // Maximum path length
#define INITIAL_DEV_ENTRIES 64 // Initial number of entries for /dev

/**
 * @brief Checks if the given path is the /dev directory
 *
 * @param path The path to check
 * @return 1 if the path is "/dev", 0 otherwise
 */
int is_dev_directory(const char *path)
{
    // Compare the given path with "/dev"
    return strcmp(path, "/dev") == 0;
}

/**
 * @brief Processes the contents of the /dev directory
 *
 * This function reads the entries in the /dev directory, creates FileCardInfo
 * structures for each entry, and stores them in the provided array.
 *
 * @param entries Pointer to an array of FileCardInfo pointers
 * @param num_entries Pointer to the number of entries in the array
 * @param current_size Pointer to the current size of the array
 * @return 0 on success, -1 on failure
 */
int handle_dev_directory(FileCardInfo **entries, int *num_entries, int *current_size)
{
    DIR *dir = opendir("/dev");  // Open the /dev directory
    if (!dir)
    {
        perror("Error opening /dev");  // Print error message if opening fails
        return -1;  // Return -1 to indicate failure
    }

    struct dirent *d_entry;  // Structure to hold directory entry information
    char full_path[MAX_PATH];  // Buffer to store full path of each entry
    struct stat st;  // Structure to hold file status information

    // Initialize entries if not already initialized
    if (*entries == NULL) {
        *entries = malloc(INITIAL_DEV_ENTRIES * sizeof(FileCardInfo));  // Allocate initial memory
        if (*entries == NULL) {
            perror("Failed to allocate initial memory for /dev entries");  // Print error if allocation fails
            closedir(dir);  // Close the directory
            return -1;  // Return -1 to indicate failure
        }
        *current_size = INITIAL_DEV_ENTRIES;  // Set current size to initial size
    }

    while ((d_entry = readdir(dir)) != NULL)  // Read each entry in the directory
    {
        if (strcmp(d_entry->d_name, ".") == 0 || strcmp(d_entry->d_name, "..") == 0)
        {
            continue; // Skip the . and .. entries
        }

        // Check if we need to allocate more memory
        if (*num_entries >= *current_size)
        {
            int new_size = *current_size * 2;  // Double the current size
            FileCardInfo *new_entries = realloc(*entries, new_size * sizeof(FileCardInfo));  // Reallocate memory
            if (!new_entries)
            {
                fprintf(stderr, "Failed to reallocate memory for /dev entries: %s\n", strerror(errno));  // Print error message
                closedir(dir);  // Close the directory
                return -1;  // Return -1 to indicate failure
            }
            *entries = new_entries;  // Update the entries pointer
            *current_size = new_size;  // Update the current size
        }

        // Construct the full path to the file in /dev
        snprintf(full_path, sizeof(full_path), "/dev/%s", d_entry->d_name);

        if (lstat(full_path, &st) == 0)  // Get file status
        {
            FileCardInfo *entry = &((*entries)[*num_entries]);  // Get pointer to current entry
            entry->name = strdup(d_entry->d_name);  // Duplicate the entry name
            if (entry->name == NULL)
            {
                perror("strdup");  // Print error message if strdup fails
                closedir(dir);  // Close the directory
                return -1;  // Return -1 to indicate failure
            }

            entry->is_directory = S_ISDIR(st.st_mode);  // Check if it's a directory
            entry->is_hidden = (d_entry->d_name[0] == '.');  // Check if it's a hidden file

            if (S_ISLNK(st.st_mode))  // Check if it's a symbolic link
            {
                entry->emoji = strdup(entry->is_directory ? "🔗📁" : "🔗");  // Use link emoji
            }
            else if (entry->is_directory)
            {
                entry->emoji = strdup("📁");  // Use folder emoji for directories
            }
            else
            {
                entry->emoji = get_dev_emoji(d_entry->d_name);  // Get appropriate emoji for device file
            }

            if (entry->emoji == NULL)
            {
                fprintf(stderr, "Failed to allocate memory for emoji: %s\n", strerror(errno));  // Print error message
                free(entry->name);  // Free the allocated name
                closedir(dir);  // Close the directory
                return -1;  // Return -1 to indicate failure
            }

            (*num_entries)++; // Increment the number of entries
        }
    }

    closedir(dir);  // Close the directory
    return 0;  // Return 0 to indicate success
}
