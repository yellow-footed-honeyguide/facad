#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "file_entry.h"
#include "emoji_utils.h"
#include "display_utils.h"
#include "dev_utils.h"
#include "args_parser.h"
#include "config.h"

#define MAX_PATH 4096
#define MAX_ENTRIES 1024

int main(int argc, char *argv[]) {
    CommandLineArgs args = parse_args(argc, argv);

    if (args.show_version) {
        print_version();
        return 0;
    }

    if (args.show_help) {
        print_help(argv[0]);
        return 0;
    }

    char current_dir[MAX_PATH];
    struct winsize w;
    int term_width;

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd() error");
        return 1;
    }

    printf("\033[1m%s\033[0m\n", current_dir);

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    term_width = w.ws_col;

    FileEntry *entries = malloc(MAX_ENTRIES * sizeof(FileEntry));
    if (entries == NULL) {
        perror("malloc() error");
        return 1;
    }
    int num_entries = 0;

    if (is_dev_directory(current_dir)) {
        handle_dev_directory(&entries, &num_entries, MAX_ENTRIES);
    } else {
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("opendir() error");
            free(entries);
            return 1;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL && num_entries < MAX_ENTRIES) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            if (create_file_entry(&entries[num_entries], entry->d_name) != 0) {
                fprintf(stderr, "Warning: Unable to get info for %s: %s\n", entry->d_name, strerror(errno));
                continue;
            }
            num_entries++;
        }
        closedir(dir);
    }

    qsort(entries, num_entries, sizeof(FileEntry), compare_file_entries);
    display_entries(entries, num_entries, term_width);

    for (int i = 0; i < num_entries; i++) {
        free_file_entry(&entries[i]);
    }
    free(entries);

    return 0;
}
