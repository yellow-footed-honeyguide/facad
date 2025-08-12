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

#include <stdbool.h>

#define MAX_FILENAME 1024
#define MAX_FILES 1000

typedef struct {
    char filename[MAX_FILENAME];
    char status;
} GitFile;

typedef struct {
    GitFile files[MAX_FILES];
    int count;
} GitFileList;

/**
 * @brief Checks if the current directory is part of a Git repository.
 *
 * This function determines whether the current working directory
 * is within a Git repository.
 *
 * @return true if the current directory is in a Git repository, false otherwise.
 */
bool is_git_repository(const char* path);

/**
 * @brief Retrieves the Git status for all files in the current repository.
 *
 * This function gets the status of all files in the current Git repository,
 * including untracked, modified, and staged files.
 *
 * @return GitFileList structure containing the status of all files in the repository.
 */
GitFileList get_git_status(const char* path);

/**
 * @brief Retrieves the name of the current Git branch.
 *
 * This function gets the name of the current Git branch in the repository.
 *
 * @return Dynamically allocated string containing the branch name, or NULL if an error occurred.
 * @note The caller is responsible for freeing the returned string.
 */
char* get_current_branch(const char* path);

#endif // GIT_UTILS_H
