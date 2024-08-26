#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <stdbool.h>

typedef struct {
    bool show_version;
    bool show_help;
} CommandLineArgs;

CommandLineArgs parse_args(int argc, char *argv[]);
void print_version(void);
void print_help(const char *program_name);

#endif // ARGS_PARSER_H
