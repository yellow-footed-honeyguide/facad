/**
 * @file git_utils.h
 * @brief Header file for Git utility functions used in facad.
 *
 * This file declares structures and functions for interacting with Git repositories.
 * It provides functionality to check if a directory is a Git repository,
 * retrieve Git status information, and get the current branch name.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef GIT_UTILS_H
#define GIT_UTILS_H

#include <stdbool.h>  // Include for boolean type

#define MAX_FILENAME 1024  // Maximum length for filenames
#define MAX_FILES 1000     // Maximum number of files to track

/**
 * @struct GitFile
 * @brief Structure to hold information about a single file's Git status.
 */
typedef struct {
    char filename[MAX_FILENAME];  // Name of the file
    char status;                  // Single character representing Git status
} GitFile;

/**
 * @struct GitFileList
 * @brief Structure to hold Git status information for multiple files.
 */
typedef struct {
    GitFile files[MAX_FILES];  // Array of GitFile structures
    int count;                 // Number of files in the list
} GitFileList;

/**
 * @brief Checks if the current directory is part of a Git repository.
 *
 * This function determines whether the current working directory
 * is within a Git repository.
 *
 * @return true if the current directory is in a Git repository, false otherwise.
 */
bool is_git_repository(void);

/**
 * @brief Retrieves the Git status for all files in the current repository.
 *
 * This function gets the status of all files in the current Git repository,
 * including untracked, modified, and staged files.
 *
 * @return GitFileList structure containing the status of all files in the repository.
 */
GitFileList get_git_status(void);

/**
 * @brief Retrieves the name of the current Git branch.
 *
 * This function gets the name of the current Git branch in the repository.
 *
 * @return Dynamically allocated string containing the branch name, or NULL if an error occurred.
 * @note The caller is responsible for freeing the returned string.
 */
char* get_current_branch(void);

#endif // GIT_UTILS_H
