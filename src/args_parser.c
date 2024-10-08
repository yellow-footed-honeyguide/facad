/**
 * @file args_parser.c
 * @brief Command-line argument parsing for facad
 *
 * This file contains functions for parsing command-line arguments
 * and displaying help and version information for the facad program.
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
 * @brief Parse command-line arguments
 *
 * This function parses the command-line arguments passed to the program
 * and sets the appropriate flags in the CommandLineArgs structure.
 *
 * @param argc The number of command-line arguments
 * @param argv An array of strings containing the command-line arguments
 * @return A CommandLineArgs structure with flags set based on the arguments
 */
CommandLineArgs parse_args(int argc, char *argv[]) {
    CommandLineArgs args = {0, 0, 0, 0, 0, NULL};  // Initialize all fields to 0 or NULL

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            args.show_version = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            args.show_help = 1;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--long") == 0) {
            args.show_longlisting = 1;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--analytics") == 0) {
            args.show_dir_analytics = 1;
        } else if (args.directory_path == NULL) {
            args.directory_path = argv[i];
        }
    }

    return args;
}

/**
 * @brief Print the version information
 *
 * This function prints the version information of the facad program.
 * The version number is defined in the config.h file.
 */
void print_version(void) {
    printf("facad version %s\n", VERSION);
}

/**
 * @brief Print the help information
 *
 * This function displays the usage information and available options
 * for the facad program.
 *
 * @param program_name The name of the program executable
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
