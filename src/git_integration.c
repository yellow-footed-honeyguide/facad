/**
 * @file git_integration.c
 * @brief Implementation of Git integration for file entries.
 *
 * This file contains functions for integrating Git status information
 * into the file entries displayed by the facad tool. It provides functionality
 * to retrieve and apply Git status to FileCardInfo structures.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include "git_integration.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>

/**
 * @brief Integrates Git status information into file entries.
 *
 * This function retrieves the Git status for files in the current directory
 * and updates the corresponding FileCardInfo structures with this information.
 *
 * @param entries Pointer to an array of FileCardInfo pointers.
 * @param num_entries Pointer to the number of entries in the array.
 * @param current_dir The current directory path.
 */
void integrate_git_status(FileCardInfo** entries, int* num_entries, const char* current_dir) {
    // Check if the current directory is a Git repository
    if (!is_git_repository()) {
        return;  // If not, return without doing anything
    }

    // Get the Git status for all files in the repository
    GitFileList git_status = get_git_status();

    // Create a copy of the current directory path
    char* base_dir = strdup(current_dir);
    // Extract the base name of the directory
    char* dir_name = basename(base_dir);

    // Iterate through all file entries
    for (int i = 0; i < *num_entries; i++) {
        // Initialize Git status as empty for each entry
        (*entries)[i].git_status[0] = '\0';
        (*entries)[i].git_status[1] = '\0';

        // Construct the relative path for the current file
        char relative_path[MAX_FILENAME];
        snprintf(relative_path, sizeof(relative_path), "%s/%s", dir_name, (*entries)[i].name);

        // Compare the file's path with Git status entries
        for (int j = 0; j < git_status.count; j++) {
            // If the paths match, update the Git status in the file entry
            if (strcmp(relative_path, git_status.files[j].filename) == 0 ||
                strcmp((*entries)[i].name, git_status.files[j].filename) == 0) {
                (*entries)[i].git_status[0] = git_status.files[j].status;
                (*entries)[i].git_status[1] = '\0';
                break;  // Stop searching once a match is found
            }
        }
    }

    // Free the memory allocated for the base directory string
    free(base_dir);
}
