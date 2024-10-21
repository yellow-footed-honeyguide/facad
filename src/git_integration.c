/**
 * @file git_integration.c
 * @brief Integration of Git status information with directory listing
 *
 * This file contains functions to integrate Git status information
 * into the file entries displayed by the facad tool. It handles
 * checking for modified files in directories and updating file
 * entry structures with Git status data.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "git_integration.h"
#include "git_utils.h"

#define MAX_PATH 4096

/**
 * @brief Check if a directory contains modified files according to Git
 *
 * @param dir_path The path to the directory to check
 * @return int 1 if the directory contains modified files, 0 otherwise
 */
static int has_modified_files(const char* dir_path) {
    char command[MAX_PATH * 3];  // Buffer to store the Git command
    char full_path[MAX_PATH];    // Buffer to store the full path of the directory

    // Get the absolute path of the directory
    if (realpath(dir_path, full_path) == NULL) {
        return 0;  // Return 0 if unable to get the real path
    }

    // Construct the Git command to check for modifications
    // The command checks the status of files in the specified directory
    snprintf(command, sizeof(command),
             "git -C \"%s\" status --porcelain \"%s\" 2>/dev/null | grep -q '^\\s*[MADRCU]'",
             full_path, full_path);

    // Execute the command and return the result
    // system() returns 0 if the command was successful (i.e., modifications found)
    return system(command) == 0;
}

/**
 * @brief Integrate Git status information into file entries
 *
 * This function updates the FileCardInfo structures with Git status
 * information for each entry. It also checks directories for modified files.
 *
 * @param entries Pointer to an array of FileCardInfo pointers
 * @param num_entries Pointer to the number of entries in the array
 * @param current_dir The current directory path
 */
void integrate_git_status(FileCardInfo** entries, int* num_entries, const char* current_dir) {
    // Check if the current directory is a Git repository
    if (!is_git_repository()) {
        return;  // If not, return without doing anything
    }

    // Get the Git status for all files in the repository
    GitFileList git_status = get_git_status();

    // Iterate through all file entries
    for (int i = 0; i < *num_entries; i++) {
        // Reset Git status and modified flag for each entry
        (*entries)[i].git_status[0] = '\0';
        (*entries)[i].git_status[1] = '\0';
        (*entries)[i].contains_modified = 0;

        // Check Git status for files
        for (int j = 0; j < git_status.count; j++) {
            // If the file name matches a file in the Git status list
            if (strcmp((*entries)[i].name, git_status.files[j].filename) == 0) {
                // Set the Git status for the file
                (*entries)[i].git_status[0] = git_status.files[j].status;
                (*entries)[i].git_status[1] = '\0';
                break;  // Stop searching once a match is found
            }
        }

        // Check directories for modified files
        if ((*entries)[i].is_directory) {
            char full_path[MAX_PATH];
            // Construct the full path of the directory
            snprintf(full_path, sizeof(full_path), "%s/%s", current_dir, (*entries)[i].name);
            // Check if the directory contains modified files
            (*entries)[i].contains_modified = has_modified_files(full_path);
        }
    }
}
