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
#include <sys/stat.h>
#include <dirent.h>
#include "git_integration.h"
#include "git_utils.h"



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
    if (!is_git_repository(current_dir)) {
        return;
    }

    GitFileList git_status = get_git_status(current_dir);
    char modified_dirs[MAX_MODIFIED_DIRS][MAX_PATH];
    int modified_dir_count = 0;

    // Collect directories with modified files
    for (int i = 0; i < git_status.count && modified_dir_count < MAX_MODIFIED_DIRS; i++) {
        char *filename = git_status.files[i].filename;
        char *slash = strchr(filename, '/');
        if (slash) {
            size_t dir_len = slash - filename;
            if (dir_len < MAX_PATH) {
                strncpy(modified_dirs[modified_dir_count], filename, dir_len);
                modified_dirs[modified_dir_count][dir_len] = '\0';
                
                // Check if directory already in list
                int found = 0;
                for (int j = 0; j < modified_dir_count; j++) {
                    if (strcmp(modified_dirs[j], modified_dirs[modified_dir_count]) == 0) {
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    modified_dir_count++;
                }
            }
        }
    }

    // Set git status for entries
    for (int i = 0; i < *num_entries; i++) {
        (*entries)[i].git_status[0] = '\0';
        (*entries)[i].contains_modified = 0;

        // Set file status
        for (int j = 0; j < git_status.count; j++) {
            if (strcmp((*entries)[i].name, git_status.files[j].filename) == 0) {
                (*entries)[i].git_status[0] = git_status.files[j].status;
                break;
            }
        }

        // Set directory modified status
        if ((*entries)[i].is_directory) {
            for (int k = 0; k < modified_dir_count; k++) {
                if (strcmp((*entries)[i].name, modified_dirs[k]) == 0) {
                    (*entries)[i].contains_modified = 1;
                    break;
                }
            }
        }
    }
}
