#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <stdbool.h>

typedef struct
{
    bool show_version;
    bool show_help;
    bool show_longlisting;
    bool show_dir_analytics;
    const char *invalid_opt;
    const char **targets;  // Can be directories, files, or patterns
    int target_count;
} CommandLineArgs;

CommandLineArgs parse_args(int argc, char *argv[]);
void free_args(CommandLineArgs *args);
void print_version(void);
void print_help(const char *program_name);

bool is_glob_pattern(const char *str);

#endif // ARGS_PARSER_H
