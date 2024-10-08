/**
 * @file args_parser.h
 * @brief Header file for command-line argument parsing in facad
 *
 * This header file declares structures and functions for parsing
 * command-line arguments and displaying help and version information
 * for the facad program.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <stdbool.h>

/**
 * @struct CommandLineArgs
 * @brief Structure to hold command-line argument flags
 *
 * This structure contains boolean flags representing various
 * command-line options for the facad program.
 */
typedef struct
{
    bool show_version;       /**< Flag to show version information */
    bool show_help;          /**< Flag to show help information */
    int show_dirinfo;        /**< Flag to show directory information (reserved for future use) */
    int show_longlisting;    /**< Flag to show detailed directory listing */
    int show_dir_analytics;  /**< Flag to show directory analytics */
    const char *directory_path;
} CommandLineArgs;

/**
 * @brief Parse command-line arguments
 *
 * @param argc The number of command-line arguments
 * @param argv An array of strings containing the command-line arguments
 * @return CommandLineArgs structure with flags set based on the arguments
 */
CommandLineArgs parse_args(int argc, char *argv[]);

/**
 * @brief Print the version information
 *
 * This function displays the version information of the facad program.
 */
void print_version(void);

/**
 * @brief Print the help information
 *
 * @param program_name The name of the program executable
 */
void print_help(const char *program_name);

#endif // ARGS_PARSER_H
