/**
 * @file args_parser.h
 * @brief Header file for command-line argument parsing in facad.
 *
 * This file declares structures and functions for parsing and handling
 * command-line arguments in the facad directory listing tool. It defines
 * the CommandLineArgs structure and related functions for argument processing.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <stdbool.h>  // Include for boolean type

/**
 * @struct CommandLineArgs
 * @brief Structure to hold parsed command-line arguments.
 *
 * This structure stores flags for various options and an array of target
 * directories or files specified in the command line.
 */
typedef struct
{
    bool show_version;        // Flag to show version information
    bool show_help;           // Flag to show help information
    bool show_longlisting;    // Flag to show detailed directory listing
    bool show_dir_analytics;  // Flag to show directory analytics
    const char *invalid_opt;  // Stores any invalid option encountered
    const char **targets;     // Array of target directories or files
    int target_count;         // Number of targets specified
} CommandLineArgs;

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
CommandLineArgs parse_args(int argc, char *argv[]);

/**
 * @brief Frees memory allocated for command-line arguments.
 *
 * This function releases the memory allocated for the targets array
 * in the CommandLineArgs structure.
 *
 * @param args Pointer to the CommandLineArgs structure to be freed.
 */
void free_args(CommandLineArgs *args);

/**
 * @brief Prints the version information of facad.
 *
 * This function displays the version number of the facad program.
 */
void print_version(void);

/**
 * @brief Prints the help information for facad.
 *
 * This function displays usage information and available options
 * for the facad program.
 *
 * @param program_name The name of the program executable.
 */
void print_help(const char *program_name);

/**
 * @brief Checks if a string is a glob pattern.
 *
 * This function determines whether a given string contains
 * wildcard characters used in glob patterns.
 *
 * @param str The string to check for glob pattern characters.
 * @return true if the string contains glob pattern characters, false otherwise.
 */
bool is_glob_pattern(const char *str);

#endif // ARGS_PARSER_H
