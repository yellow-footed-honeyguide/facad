/**
 * @file git_utils.c
 * @brief Implementation of Git utility functions for facad.
 *
 * This file contains utility functions for interacting with Git repositories.
 * It provides functionality to check if the current directory is a Git repository,
 * retrieve Git status information, and get the current branch name.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "git_utils.h"

/**
 * @brief Checks if the current directory is part of a Git repository.
 *
 * This function executes the 'git rev-parse' command to determine
 * if the current working directory is within a Git repository.
 *
 * @return true if the current directory is in a Git repository, false otherwise.
 */
bool is_git_repository() {
    // Execute 'git rev-parse' command and redirect output to /dev/null
    FILE* fp = popen("git rev-parse --is-inside-work-tree 2>/dev/null", "r");
    if (fp == NULL) {
        return false;  // Return false if command execution failed
    }

    char buffer[128];  // Buffer to store command output
    // Read command output and check if it's "true"
    bool result = fgets(buffer, sizeof(buffer), fp) != NULL && strcmp(buffer, "true\n") == 0;
    pclose(fp);  // Close the command pipe
    return result;  // Return the result
}

/**
 * @brief Parses a Git status string and returns a single character status code.
 *
 * This function interprets the two-character Git status string and
 * returns a single character representing the file's status.
 *
 * @param status_str Two-character Git status string.
 * @return Single character representing the file's Git status.
 */
static char parse_status(const char* status_str) {
    if (status_str[0] == '?' && status_str[1] == '?') return '?';  // Untracked
    return status_str[0] != ' ' ? status_str[0] : status_str[1];   // Modified, Staged, etc.
}

/**
 * @brief Retrieves the Git status for all files in the current repository.
 *
 * This function executes 'git status' command and parses its output to create
 * a list of files with their corresponding Git status.
 *
 * @return GitFileList structure containing the status of all files in the repository.
 */
GitFileList get_git_status() {
    GitFileList list = {0};  // Initialize an empty GitFileList

    // Execute 'git status' command
    FILE* fp = popen("git status -s", "r");
    if (fp == NULL) {
        return list;  // Return empty list if command execution failed
    }

    char line[MAX_FILENAME + 3];  // Buffer to store each line of output
    // Read each line of the command output
    while (fgets(line, sizeof(line), fp) != NULL && list.count < MAX_FILES) {
        if (strlen(line) > 3) {  // Ensure the line has enough characters
            GitFile file;
            file.status = parse_status(line);  // Parse the status
            // Copy the filename, skipping the first 3 characters (status and space)
            strncpy(file.filename, line + 3, MAX_FILENAME - 1);
            file.filename[strcspn(file.filename, "\n")] = 0;  // Remove newline
            list.files[list.count++] = file;  // Add file to the list
        }
    }

    pclose(fp);  // Close the command pipe
    return list;  // Return the populated list
}

/**
 * @brief Retrieves the name of the current Git branch.
 *
 * This function executes 'git rev-parse' command to get the name of the
 * current Git branch.
 *
 * @return Dynamically allocated string containing the branch name, or NULL if an error occurred.
 * @note The caller is responsible for freeing the returned string.
 */
char* get_current_branch() {
    // Execute 'git rev-parse' command to get the branch name
    FILE* fp = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
    if (fp == NULL) {
        return NULL;  // Return NULL if command execution failed
    }

    char* branch = malloc(256);  // Allocate memory for the branch name
    if (fgets(branch, 256, fp) == NULL) {  // Read the branch name
        free(branch);  // Free allocated memory if read failed
        pclose(fp);    // Close the command pipe
        return NULL;   // Return NULL to indicate failure
    }

    pclose(fp);  // Close the command pipe
    branch[strcspn(branch, "\n")] = 0;  // Remove newline character
    return branch;  // Return the branch name
}
