#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <stdbool.h> // Include for the 'bool' type

/**
 * Structure to hold command line arguments flags.
 */
typedef struct
{
    bool show_version; // Flag indicating whether to show version
    bool show_help;    // Flag indicating whether to show help
    int show_dirinfo;
    int show_longlisting;
    int show_dir_analytics;
} CommandLineArgs;

/**
 * Parses command line arguments and returns a CommandLineArgs structure.
 *
 * @param argc The number of arguments.
 * @param argv The array of arguments.
 * @return A CommandLineArgs structure with the appropriate flags set.
 */
CommandLineArgs parse_args(int argc, char *argv[]);

/**
 * Prints the version information of the program.
 */
void print_version(void);

/**
 * Prints the help information of the program.
 *
 * @param program_name The name of the program to display in the help message.
 */
void print_help(const char *program_name);

#endif // ARGS_PARSER_H
