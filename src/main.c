/**
 * @file main.c
 * @brief Main entry point for the facad directory listing tool.
 *
 * This file contains the main function and core logic for the facad tool,
 * which provides a colorful and feature-rich directory listing in the terminal.
 * It implements dynamic memory allocation for directory entries and follows
 * best practices for code organization and documentation.
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
#include <sys/ioctl.h>

#include "file_card.h"
#include "dir_config.h"
#include "args_parser.h"
#include "longlisting.h"
#include "dir_analytics.h"
#include "display_utils.h"
#include "dev_dir_utils.h"

#define MAX_PATH 4096        // Define maximum path length
#define INITIAL_ENTRIES 512  // Define initial number of directory entries

/**
 * @brief Dynamically allocates memory for directory entries.
 *
 * This function allocates or reallocates memory for directory entries.
 * It starts with INITIAL_ENTRIES and doubles the size when needed.
 *
 * @param entries Pointer to the array of FileCardInfo structures.
 * @param current_size Pointer to the current size of the array.
 * @return 0 on success, -1 on failure.
 */
static int allocate_entries(FileCardInfo **entries, int *current_size) {
    // Calculate new size (double the current size or use initial size if NULL)
    int new_size = (*entries == NULL) ? INITIAL_ENTRIES : (*current_size * 2);

    // Reallocate memory for entries
    FileCardInfo *new_entries = realloc(*entries, new_size * sizeof(FileCardInfo));

    // Check if reallocation was successful
    if (new_entries == NULL) {
        perror("Failed to allocate memory for entries");
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
 * This function reads directory entries, creates FileCardInfo structures,
 * and dynamically allocates memory as needed.
 *
 * @param dir_path Path to the directory to process.
 * @param entries Pointer to the array of FileCardInfo structures.
 * @param num_entries Pointer to the number of entries processed.
 * @param current_size Pointer to the current size of the entries array.
 * @return 0 on success, -1 on failure.
 */
static int process_directory(const char *dir_path, FileCardInfo **entries, int *num_entries, int *current_size) {
    // Open the directory
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Error opening directory");
        return -1;  // Return -1 to indicate failure
    }

    struct dirent *entry;
    // Read directory entries one by one
    while ((entry = readdir(dir)) != NULL) {
        // Skip '.' and '..' entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Check if we need to allocate more memory
        if (*num_entries >= *current_size) {
            if (allocate_entries(entries, current_size) != 0) {
                closedir(dir);
                return -1;  // Return -1 if memory allocation fails
            }
        }

        // Create a file entry for the current directory item
        if (create_file_entry(&(*entries)[*num_entries], entry->d_name) != 0) {
            fprintf(stderr, "Warning: Unable to get info for %s: %s\n", entry->d_name, strerror(errno));
            continue;  // Skip this entry and continue with the next one
        }

        // Increment the number of entries
        (*num_entries)++;
    }

    // Close the directory
    closedir(dir);
    return 0;  // Return 0 to indicate success
}

/**
 * @brief Main function of the facad tool.
 *
 * This function parses command-line arguments, determines the current working
 * directory, and handles different modes of operation based on the provided flags.
 * It implements dynamic memory allocation for directory entries and proper error handling.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return EXIT_SUCCESS on successful execution, EXIT_FAILURE on error.
 */
int main(int argc, char *argv[]) {
    // Parse command-line arguments
    CommandLineArgs args = parse_args(argc, argv);

    // Check if version information should be displayed
    if (args.show_version) {
        print_version();
        return EXIT_SUCCESS;
    }

    // Check if help information should be displayed
    if (args.show_help) {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }

    // Change to specified directory if provided
    if (args.dir_path) {
        if (chdir(args.dir_path) != 0) {
            // Print error message if changing directory fails
            fprintf(stderr, "%s: error changing to directory '%s': %s\n",
                    argv[0], args.dir_path, strerror(errno));
            return EXIT_FAILURE;
        }
    } else if (args.invalid_opt) {
        // Handle invalid option
        fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], args.invalid_opt);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Get current working directory
    char current_dir[MAX_PATH];
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd() error");
        return EXIT_FAILURE;
    }

    // Check if long listing should be displayed
    if (args.show_longlisting) {
        print_longlisting(".");
        return EXIT_SUCCESS;
    }

    // Check if directory analytics should be displayed
    if (args.show_dir_analytics) {
        print_dir_analytics(".");
        return EXIT_SUCCESS;
    }

    // Print current directory in bold
    printf("\033[1m%s\033[0m\n", current_dir);

    // Get terminal width
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;

    // Initialize variables for storing directory entries
    FileCardInfo *entries = NULL;
    int num_entries = 0;
    int current_size = 0;

    // Check if current directory is /dev and handle accordingly
    if (is_dev_directory(current_dir)) {
        if (handle_dev_directory(&entries, &num_entries, &current_size) != 0) {
            fprintf(stderr, "Error processing /dev directory\n");
            return EXIT_FAILURE;
        }
    } else {
        // Process regular directory
        if (process_directory(".", &entries, &num_entries, &current_size) != 0) {
            fprintf(stderr, "Error processing directory\n");
            free(entries);
            return EXIT_FAILURE;
        }
    }

    // Check if any entries were found
    if (num_entries == 0) {
        fprintf(stderr, "No entries found in the directory\n");
        free(entries);
        return EXIT_FAILURE;
    }

    // Sort the entries
    qsort(entries, num_entries, sizeof(FileCardInfo), compare_file_entries);

    // Display the sorted entries
    display_entries(entries, num_entries, term_width, current_dir);

    // Free memory for each entry
    for (int i = 0; i < num_entries; i++) {
        free_file_entry(&entries[i]);
    }
    // Free the entire entries array
    free(entries);

    return EXIT_SUCCESS;  // Return success
}
