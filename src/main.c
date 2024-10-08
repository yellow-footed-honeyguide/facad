/**
 * @file main.c
 * @brief Main entry point for the facad directory listing tool.
 *
 * This file contains the main function and core logic for the facad tool,
 * which provides a colorful and feature-rich directory listing in the terminal.
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

#include "dev_utils.h"
#include "file_card.h"
#include "args_parser.h"
#include "longlisting.h"
#include "dir_analytics.h"
#include "display_utils.h"
#include "directory_config.h"

#define MAX_PATH 4096 /**< Maximum path length */
#define MAX_ENTRIES 8192 /**< Maximum number of directory entries */


/**
 * @brief Main function of the facad tool.
 *
 * This function parses command-line arguments, determines the current working
 * directory, and handles different modes of operation based on the provided flags.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return 0 on successful execution, 1 on error.
 */
int main(int argc, char *argv[])
{
    CommandLineArgs args = parse_args(argc, argv); // Parse CLI args
    if (args.show_version)
    {                    // If --version flag is set
        print_version(); // Print version info
        return 0;        // Exit program
    }

    if (args.show_help)
    {                        // If --help flag is set
        print_help(argv[0]); // Print help info
        return 0;            // Exit program
    }

    if (args.dir_path) {
        if (chdir(args.dir_path) != 0) {
            if (errno == ENOENT) {
                fprintf(stderr, "%s: cannot access '%s': No such file or directory\n",
                        argv[0], args.dir_path);
             }
            else {
                fprintf(stderr, "%s: error changing to directory '%s': %s\n",
                        argv[0], args.dir_path, strerror(errno));
            }
            exit(EXIT_FAILURE);
        }
    } else if (args.invalid_opt) {
        fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], args.invalid_opt);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char current_dir[MAX_PATH]; // Buffer to store current directory path
    struct winsize w;           // Structure to hold terminal window size
    int term_width;             // Terminal width

    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {                             // Get current working directory
        perror("getcwd() error"); // Print error message if getcwd() fails
        return 1;                 // Exit with error code
    }

    if (args.show_longlisting) { // If -l flag is set
        print_longlisting(".");  // Print longlisting info
        return 0;                // Exit program
    }

    if (args.show_dir_analytics) { // If -a flag is set
        print_dir_analytics(".");  // Print dir analitics info
        return 0;                  // Exit program
    }

    // Print current directory in bold
    printf("\033[1m%s\033[0m\n", current_dir);

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Get terminal window size
    term_width = w.ws_col;                // Store terminal width

    // Allocate memory for file entries
    FileCardInfo *entries = malloc(MAX_ENTRIES * sizeof(FileCardInfo));
    if (entries == NULL)
    {                             // Check if memory allocation failed
        perror("malloc() error"); // Print error message
        return 1;                 // Exit with error code
    }

    int num_entries = 0; // Counter for number of entries

    if (is_dev_directory(current_dir))
    { // Check if current directory is /dev
        // Handle /dev directory specially
        handle_dev_directory(&entries, &num_entries, MAX_ENTRIES);
    }
    else
    {
        DIR *dir = opendir("."); // Open current directory
        if (dir == NULL)
        {                              // Check if directory opening failed
            perror("opendir() error"); // Print error message
            free(entries);             // Free allocated memory
            return 1;                  // Exit with error code
        }

        struct dirent *entry; // Pointer to directory entry
        while ((entry = readdir(dir)) != NULL && num_entries < MAX_ENTRIES)
        { // Read directory entries
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            { // Skip . and .. entries
                continue;
            }

            if (create_file_entry(&entries[num_entries], entry->d_name) != 0)
            { // Create file entry
                // Print warning if creation fails
                fprintf(stderr, "Warning: Unable to get info for %s: %s\n", entry->d_name, strerror(errno));
                continue; // Skip to next entry
            }
            num_entries++; // Increment entry count
        }
        closedir(dir); // Close directory
    }

    // Sort entries
    qsort(entries, num_entries, sizeof(FileCardInfo), compare_file_entries);

    // Display sorted entries
    display_entries(entries, num_entries, term_width, current_dir);

    // Free each entry
    for (int i = 0; i < num_entries; i++)
    {
        free_file_entry(&entries[i]);
    }
    free(entries); // Free entries array

    return 0; // Exit successfully
}
