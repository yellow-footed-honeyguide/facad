/**
 * @file main.c
 * @brief Main entry point for the facad directory listing tool.
 *
 * This file contains the main function and core logic for the facad tool,
 * which provides a colorful and feature-rich directory listing in the terminal.
 * It handles command-line arguments, processes directories or files,
 * and displays the results using various output formats.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "git_utils.h"
#include "file_card.h"
#include "dir_config.h"
#include "args_parser.h"
#include "longlisting.h"
#include "dir_analytics.h"
#include "display_utils.h"
#include "dev_dir_utils.h"
#include "git_integration.h"

#define MAX_PATH 4096        // Maximum path length
#define INITIAL_ENTRIES 512  // Initial number of directory entries to allocate

/**
 * @brief Dynamically allocates or reallocates memory for directory entries.
 *
 * This function manages the memory for the array of FileCardInfo structures.
 * It starts with INITIAL_ENTRIES and doubles the size when more space is needed.
 *
 * @param entries Pointer to the array of FileCardInfo structures.
 * @param current_size Pointer to the current size of the array.
 * @return 0 on success, -1 on failure.
 */
static int allocate_entries(FileCardInfo **entries, int *current_size) {
    // Calculate new size (double the current size or use initial size if NULL)
    int new_size = (*entries == NULL) ? INITIAL_ENTRIES : (*current_size * 2);

    // Attempt to reallocate memory for entries
    FileCardInfo *new_entries = realloc(*entries, new_size * sizeof(FileCardInfo));

    // Check if reallocation was successful
    if (new_entries == NULL) {
        perror("Failed to allocate memory for entries"); // Print error message
        return -1;  // Return -1 to indicate failure
    }

    // Update entries pointer and current size
    *entries = new_entries;
    *current_size = new_size;
    return 0;  // Return 0 to indicate success
}

/**
 * @brief Processes directory entries and populates the entries array.
 *
 * This function reads directory contents, creates FileCardInfo structures for each entry,
 * and dynamically allocates memory as needed. It also integrates Git status information.
 *
 * @param dir_path Path to the directory to process.
 * @param entries Pointer to the array of FileCardInfo structures.
 * @param num_entries Pointer to the number of entries processed.
 * @param current_size Pointer to the current size of the entries array.
 * @return 0 on success, -1 on failure.
 */
static int process_directory(const char *dir_path, FileCardInfo **entries, int *num_entries, int *current_size) {
    DIR *dir = opendir(dir_path); // Open the directory
    if (dir == NULL) {
        perror("Error opening directory"); // Print error message if directory can't be opened
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) { // Loop through each entry in the directory
        // Skip '.' and '..' entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Check if we need to allocate more memory
        if (*num_entries >= *current_size) {
            if (allocate_entries(entries, current_size) != 0) {
                closedir(dir); // Close the directory before returning
                return -1;
            }
        }

        // Construct full path for the current entry
        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        // Create a file entry and add it to the array
        if (create_file_entry(&(*entries)[*num_entries], entry->d_name) != 0) {
            fprintf(stderr, "Warning: Unable to get info for %s: %s\n", full_path, strerror(errno));
            continue;
        }

        (*num_entries)++; // Increment the number of entries
    }

    closedir(dir); // Close the directory

    // Integrate Git status information for the entries
    integrate_git_status(entries, num_entries, dir_path);

    return 0; // Return 0 to indicate success
}

/**
 * @brief Processes files or patterns and populates the entries array.
 *
 * This function handles file patterns (including globs) and individual files,
 * creating FileCardInfo structures for matching entries.
 *
 * @param patterns Array of file patterns or names to process.
 * @param pattern_count Number of patterns in the array.
 * @param entries Pointer to the array of FileCardInfo structures.
 * @param num_entries Pointer to the number of entries processed.
 * @param current_size Pointer to the current size of the entries array.
 * @return 0 on success, -1 on failure.
 */
static int process_files_or_patterns(const char **patterns, int pattern_count, FileCardInfo **entries, int *num_entries, int *current_size) {
    DIR *dir;
    struct dirent *entry;
    char full_path[MAX_PATH];

    dir = opendir("."); // Open the current directory
    if (dir == NULL) {
        perror("Error opening current directory"); // Print error message if directory can't be opened
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) { // Loop through each entry in the directory
        bool match = false;
        // Check if the entry matches any of the provided patterns
        for (int i = 0; i < pattern_count; i++) {
            if (fnmatch(patterns[i], entry->d_name, 0) == 0) {
                match = true;
                break;
            }
        }

        if (match) { // If the entry matches a pattern
            // Check if we need to allocate more memory
            if (*num_entries >= *current_size) {
                if (allocate_entries(entries, current_size) != 0) {
                    closedir(dir); // Close the directory before returning
                    return -1;
                }
            }

            // Create a file entry for the matching file
            snprintf(full_path, sizeof(full_path), "%s", entry->d_name);
            if (create_file_entry(&(*entries)[*num_entries], full_path) != 0) {
                fprintf(stderr, "Warning: Unable to get info for %s: %s\n", full_path, strerror(errno));
                continue;
            }

            (*num_entries)++; // Increment the number of entries
        }
    }

    closedir(dir); // Close the directory
    return *num_entries > 0 ? 0 : -1; // Return 0 if entries were found, -1 otherwise
}

/**
 * @brief Checks if a given path is a directory.
 *
 * @param path The path to check.
 * @return 1 if the path is a directory, 0 otherwise.
 */
static int is_directory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) // Get file status
        return 0; // Return 0 if stat fails
    return S_ISDIR(statbuf.st_mode); // Return 1 if it's a directory, 0 otherwise
}

/**
 * @brief Processes a single target (directory, file, or pattern).
 *
 * This function handles different types of targets: directories, glob patterns,
 * and individual files. It populates the entries array accordingly.
 *
 * @param target The target to process (directory path, file path, or pattern).
 * @param entries Pointer to the array of FileCardInfo structures.
 * @param num_entries Pointer to the number of entries processed.
 * @param current_size Pointer to the current size of the entries array.
 * @return 0 on success, -1 on failure.
 */
static int process_target(const char *target, FileCardInfo **entries, int *num_entries, int *current_size) {
    if (is_directory(target)) { // If the target is a directory
        char previous_dir[MAX_PATH];
        if (getcwd(previous_dir, sizeof(previous_dir)) == NULL) { // Get current working directory
            perror("getcwd() error");
            return -1;
        }

        if (chdir(target) != 0) { // Change to the target directory
            fprintf(stderr, "Error changing to directory '%s': %s\n", target, strerror(errno));
            return -1;
        }

        int result = process_directory(".", entries, num_entries, current_size); // Process the directory

        // Change back to the previous directory
        if (chdir(previous_dir) != 0) {
            fprintf(stderr, "Error changing back to previous directory: %s\n", strerror(errno));
            return -1;
        }

        return result;
    } else if (is_glob_pattern(target)) { // If the target is a glob pattern
        return process_files_or_patterns(&target, 1, entries, num_entries, current_size);
    } else { // Assume it's a single file
        // Check if we need to allocate more memory
        if (*num_entries >= *current_size) {
            if (allocate_entries(entries, current_size) != 0) {
                return -1;
            }
        }
        // Create a file entry for the single file
        if (create_file_entry(&(*entries)[*num_entries], target) != 0) {
            fprintf(stderr, "Unable to get info for %s: %s\n", target, strerror(errno));
            return -1;
        }
        (*num_entries)++; // Increment the number of entries
    }
    return 0; // Return 0 to indicate success
}

/**
 * @brief Checks if the targets are glob patterns or specific files.
 *
 * This function determines whether the given targets include glob patterns
 * or specific files, as opposed to just directories.
 *
 * @param targets Array of target strings.
 * @param target_count Number of targets in the array.
 * @return 1 if glob patterns or specific files are present, 0 otherwise.
 */
static int is_glob_or_specific_files(const char **targets, int target_count) {
    for (int i = 0; i < target_count; i++) {
        if (is_glob_pattern(targets[i]) || !is_directory(targets[i])) {
            return 1; // Return 1 if a glob pattern or non-directory is found
        }
    }
    return 0; // Return 0 if all targets are directories
}

/**
 * @brief Main function of the facad program.
 *
 * This function serves as the entry point for the facad directory listing tool.
 * It parses command-line arguments, processes directories or files based on the input,
 * and displays the results using the appropriate format (standard, long listing, or analytics).
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return EXIT_SUCCESS on successful execution, EXIT_FAILURE on error.
 */
int main(int argc, char *argv[]) {
    CommandLineArgs args = parse_args(argc, argv); // Parse command-line arguments

    // Handle version display request
    if (args.show_version) {
        print_version();
        free_args(&args);
        return EXIT_SUCCESS;
    }

    // Handle help display request
    if (args.show_help) {
        print_help(argv[0]);
        free_args(&args);
        return EXIT_SUCCESS;
    }

    // Handle invalid options
    if (args.invalid_opt) {
        fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], args.invalid_opt);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        free_args(&args);
        return EXIT_FAILURE;
    }

    // Get current working directory
    char current_dir[MAX_PATH];
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd() error");
        free_args(&args);
        return EXIT_FAILURE;
    }

    // Handle long listing request
    if (args.show_longlisting) {
        if (args.target_count > 0 && is_glob_or_specific_files(args.targets, args.target_count)) {
            print_longlisting_files(args.targets, args.target_count);
        } else {
            print_longlisting(args.target_count > 0 ? args.targets[0] : ".");
        }
        free_args(&args);
        return EXIT_SUCCESS;
    }

    // Handle directory analytics request
    if (args.show_dir_analytics) {
        print_dir_analytics(args.target_count > 0 ? args.targets[0] : ".");
        free_args(&args);
        return EXIT_SUCCESS;
    }

    // Get terminal width for display formatting
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;

    // Initialize variables for storing file entries
    FileCardInfo *entries = NULL;
    int num_entries = 0;
    int current_size = 0;

    // Set up display path
    char display_path[MAX_PATH];
    strcpy(display_path, current_dir);

    int show_path = 1; // Flag to determine if path should be displayed

    // Process targets if provided, otherwise process current directory
    if (args.target_count > 0) {
        show_path = 0; // Don't show path for specific targets
        for (int i = 0; i < args.target_count; i++) {
            // Get real path if possible
            char *real_path = realpath(args.targets[i], NULL);
            if (real_path != NULL) {
                strcpy(display_path, real_path);
                free(real_path);
            } else {
                strcpy(display_path, args.targets[i]);
            }

            // Process the target
            if (process_target(args.targets[i], &entries, &num_entries, &current_size) != 0) {
                fprintf(stderr, "Error processing '%s'\n", args.targets[i]);
                goto cleanup; // Jump to cleanup if there's an error
            }
        }
    } else {
        // Check if current directory is /dev and handle accordingly
        if (is_dev_directory(current_dir)) {
            strcpy(display_path, "/dev");
            if (handle_dev_directory(&entries, &num_entries, &current_size) != 0) {
                fprintf(stderr, "Error processing /dev directory\n");
                goto cleanup;
            }
        } else {
            // Process current directory
            if (process_directory(".", &entries, &num_entries, &current_size) != 0) {
                fprintf(stderr, "Error processing current directory\n");
                goto cleanup;
            }
        }
    }

    // Handle case when no entries are found
    if (num_entries == 0) {
        char current_dir[MAX_PATH];
        if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
            // Display current directory if it matches the display path
            if (strcmp(current_dir, display_path) == 0) {
                printf("\033[1m%s\033[0m\n", current_dir);
            }
            fprintf(stderr, "🚫 No files found\n");
        } else {
            fprintf(stderr, "🚫 No files found\n");
        }
    } else {
        // Sort the entries alphabetically
        qsort(entries, num_entries, sizeof(FileCardInfo), compare_file_entries);



        // Display the entries using the appropriate format
        display_entries(entries, num_entries, term_width, display_path, show_path);
    }

cleanup:
    // Free allocated memory for each entry
    for (int i = 0; i < num_entries; i++) {
        free_file_entry(&entries[i]);
    }
    free(entries); // Free the main entries array
    free_args(&args); // Free command-line arguments

    // Return success if entries were found, failure otherwise
    return num_entries > 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
