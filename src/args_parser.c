/**
 * @file args_parser.c
 * @brief Implementation of command-line argument parsing for facad.
 *
 * This file contains functions for parsing command-line arguments,
 * identifying options, and handling version and help requests.
 * It also includes utility functions for working with file patterns.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "args_parser.h"

/**
 * @brief Checks if a string is a glob pattern.
 *
 * This function determines whether a given string contains
 * wildcard characters used in glob patterns.
 *
 * @param str The string to check for glob pattern characters.
 * @return true if the string contains glob pattern characters, false otherwise.
 */
bool is_glob_pattern(const char *str) {
    // Check for the presence of wildcard characters
    return (strchr(str, '*') != NULL || strchr(str, '?') != NULL || strchr(str, '[') != NULL);
}

/**
 * @brief Parses command-line arguments.
 *
 * This function processes the command-line arguments, setting appropriate flags
 * in the CommandLineArgs structure and collecting non-option arguments as targets.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return A CommandLineArgs structure containing the parsed arguments.
 */
CommandLineArgs parse_args(int argc, char *argv[]) {
    CommandLineArgs args = {0};  // Initialize the args structure with zeros
    args.targets = malloc(sizeof(char*) * (argc - 1));  // Allocate memory for targets
    args.target_count = 0;  // Initialize target count to 0

    // Loop through all arguments starting from index 1 (skip program name)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            args.show_version = true;  // Set flag to show version
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            args.show_help = true;  // Set flag to show help
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--long") == 0) {
            args.show_longlisting = true;  // Set flag for long listing
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--analytics") == 0) {
            args.show_dir_analytics = true;  // Set flag for directory analytics
        } else if (argv[i][0] == '-') {
            args.invalid_opt = argv[i];  // Store invalid option
            return args;  // Return immediately on invalid option
        } else {
            args.targets[args.target_count++] = argv[i];  // Store non-option argument as target
        }
    }

    return args;  // Return the populated CommandLineArgs structure
}

/**
 * @brief Frees memory allocated for command-line arguments.
 *
 * This function releases the memory allocated for the targets array
 * in the CommandLineArgs structure.
 *
 * @param args Pointer to the CommandLineArgs structure to be freed.
 */
void free_args(CommandLineArgs *args) {
    if (args->targets) {
        free(args->targets);  // Free the targets array
        args->targets = NULL;  // Set pointer to NULL to prevent double-free
    }
}

/**
 * @brief Prints the version information of facad.
 *
 * This function displays the version number of the facad program,
 * which is defined in the config.h file.
 */
void print_version(void) {
    printf("facad version %s\n", VERSION);  // Print version number
}

/**
 * @brief Prints the help information for facad.
 *
 * This function displays usage information and available options
 * for the facad program.
 *
 * @param program_name The name of the program executable.
 */
void print_help(const char *program_name) {
    printf("Usage: %s [OPTION] [DIRECTORY]\n", program_name);
    printf("List directory contents with colorful emojis.\n\n");
    printf("Options:\n");
    printf("  -h, --help      display this help and exit\n");
    printf("  -v, --version   output version information and exit\n");
    printf("  -l, --long      display detailed directory listing\n");
    printf("  -a, --analytics display directory analytics\n");
    printf("\nIf DIRECTORY is not specified, the current directory is used.\n");
}
