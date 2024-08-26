#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args_parser.h"
#include "config.h"  // Include the generated config.h

CommandLineArgs parse_args(int argc, char *argv[]) {
    CommandLineArgs args = {false, false};

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            args.show_version = true;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            args.show_help = true;
        }
    }

    return args;
}

void print_version() {
    printf("facad version %s\n", VERSION);
}

void print_help(const char *program_name) {
    printf("Usage: %s [OPTION]\n", program_name);
    printf("List directory contents with colorful emojis.\n\n");
    printf("Options:\n");
    printf("  -h, --help     display this help and exit\n");
    printf("  -v, --version  output version information and exit\n");
}
