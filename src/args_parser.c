#include <string.h>
#include <stdio.h>
#include "args_parser.h"
#include "config.h"

CommandLineArgs parse_args(int argc, char *argv[]) {
    CommandLineArgs args = {0, 0, 0, 0, 0};  // Initialize all fields to 0

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            args.show_version = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            args.show_help = 1;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--long") == 0) {
            args.show_longlisting = 1;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--analytics") == 0) {
            args.show_dir_analytics = 1;
        }
    }

    return args;
}

void print_version(void) {
    printf("facad version %s\n", VERSION);
}

void print_help(const char *program_name) {
    printf("Usage: %s [OPTION]\n", program_name);
    printf("List directory contents with colorful emojis.\n\n");
    printf("Options:\n");
    printf("  -h, --help      display this help and exit\n");
    printf("  -v, --version   output version information and exit\n");
    printf("  -l, --long      display detailed directory listing\n");
    printf("  -a, --analytics display directory analytics\n");
}
