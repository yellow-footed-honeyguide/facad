//  src/args_parser.c

#include <stdio.h>
#include <string.h>

#include "args_parser.h" // Header file with the definition of CommandLineArgs structure and parse_args function
#include "config.h"      // Includes the generated config.h file with the VERSION definition

/**
 * Function to parse command line arguments.
 * @param argc The number of arguments.
 * @param argv The array of arguments.
 * @return A CommandLineArgs structure with show_version and show_help flags set.
 */
CommandLineArgs parse_args(int argc, char *argv[])
{
    CommandLineArgs args = {false, false}; // Initialize the args structure with default values

    for (int i = 1; i < argc; i++)
    { // Iterate over command line arguments starting from the first one
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
        {
            args.show_version = true; // Set the version flag if the appropriate argument is passed
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            args.show_help = true; // Set the help flag if the appropriate argument is passed
        }
    }

    return args; // Return the structure with the flags set
}

/**
 * Function to print the program version information.
 */
void print_version(void)
{
    printf("facad version %s\n", VERSION); // Output the program version
}

/**
 * Function to print the help information of the program.
 * @param program_name The name of the program to be displayed in the help message.
 */
void print_help(const char *program_name)
{
    printf("Usage: %s [OPTION]\n", program_name);
    printf("List directory contents with colorful emojis.\n\n");
    printf("Options:\n");
    printf("  -h, --help     display this help and exit\n");
    printf("  -v, --version  output version information and exit\n");
}
