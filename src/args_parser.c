#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "args_parser.h"

bool is_glob_pattern(const char *str) {
    return (strchr(str, '*') != NULL || strchr(str, '?') != NULL || strchr(str, '[') != NULL);
}

CommandLineArgs parse_args(int argc, char *argv[]) {
    CommandLineArgs args = {0};
    args.targets = malloc(sizeof(char*) * (argc - 1));  // Allocate max possible size
    args.target_count = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            args.show_version = true;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            args.show_help = true;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--long") == 0) {
            args.show_longlisting = true;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--analytics") == 0) {
            args.show_dir_analytics = true;
        } else if (argv[i][0] == '-') {
            args.invalid_opt = argv[i];
            return args;  // Return immediately on invalid option
        } else {
            args.targets[args.target_count++] = argv[i];
        }
    }

    return args;
}

void free_args(CommandLineArgs *args) {
    if (args->targets) {
        free(args->targets);
        args->targets = NULL;
    }
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

