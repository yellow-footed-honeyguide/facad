/**
 * @file main.c
 * @brief Main entry point for the facad directory listing tool.
 *
 * This file contains the main function and core logic for the facad tool,
 * which provides a colorful and feature-rich directory listing in the terminal.
 * It implements dynamic memory allocation for directory entries and follows
 * best practices for code organization and documentation.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <fnmatch.h>
#include <sys/stat.h>

#include "file_card.h"
#include "dir_config.h"
#include "args_parser.h"
#include "longlisting.h"
#include "dir_analytics.h"
#include "display_utils.h"
#include "dev_dir_utils.h"

#include "git_integration.h"
#include "git_utils.h"

#define MAX_PATH 4096        // Define maximum path length
#define INITIAL_ENTRIES 512  // Define initial number of directory entries

/**
 * @brief Dynamically allocates memory for directory entries.
 *
 * This function allocates or reallocates memory for directory entries.
 * It starts with INITIAL_ENTRIES and doubles the size when needed.
 *
 * @param entries Pointer to the array of FileCardInfo structures.
 * @param current_size Pointer to the current size of the array.
 * @return 0 on success, -1 on failure.
 */
static int allocate_entries(FileCardInfo **entries, int *current_size) {
    // Calculate new size (double the current size or use initial size if NULL)
    int new_size = (*entries == NULL) ? INITIAL_ENTRIES : (*current_size * 2);

    // Reallocate memory for entries
    FileCardInfo *new_entries = realloc(*entries, new_size * sizeof(FileCardInfo));

    // Check if reallocation was successful
    if (new_entries == NULL) {
        perror("Failed to allocate memory for entries");
        return -1;  // Return -1 to indicate failure
    }

    // Update entries pointer and current size
    *entries = new_entries;
    *current_size = new_size;
    return 0;  // Return 0 to indicate success
}

/**
 * @brief Processes directory entries and populates the entries array.
 *
 * This function reads directory entries, creates FileCardInfo structures,
 * and dynamically allocates memory as needed.
 *
 * @param dir_path Path to the directory to process.
 * @param entries Pointer to the array of FileCardInfo structures.
 * @param num_entries Pointer to the number of entries processed.
 * @param current_size Pointer to the current size of the entries array.
 * @return 0 on success, -1 on failure.
 */
static int process_directory(const char *dir_path, FileCardInfo **entries, int *num_entries, int *current_size) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Error opening directory");
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (*num_entries >= *current_size) {
            if (allocate_entries(entries, current_size) != 0) {
                closedir(dir);
                return -1;
            }
        }

        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        if (create_file_entry(&(*entries)[*num_entries], entry->d_name) != 0) {
            fprintf(stderr, "Warning: Unable to get info for %s: %s\n", full_path, strerror(errno));
            continue;
        }

        (*num_entries)++;
    }

    closedir(dir);

    integrate_git_status(entries, num_entries, dir_path);

    return 0;
}

static int process_files_or_patterns(const char **patterns, int pattern_count, FileCardInfo **entries, int *num_entries, int *current_size) {
    DIR *dir;
    struct dirent *entry;
    char full_path[MAX_PATH];

    dir = opendir(".");
    if (dir == NULL) {
        perror("Error opening current directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        bool match = false;
        for (int i = 0; i < pattern_count; i++) {
            if (fnmatch(patterns[i], entry->d_name, 0) == 0) {
                match = true;
                break;
            }
        }

        if (match) {
            if (*num_entries >= *current_size) {
                if (allocate_entries(entries, current_size) != 0) {
                    closedir(dir);
                    return -1;
                }
            }

            snprintf(full_path, sizeof(full_path), "%s", entry->d_name);
            if (create_file_entry(&(*entries)[*num_entries], full_path) != 0) {
                fprintf(stderr, "Warning: Unable to get info for %s: %s\n", full_path, strerror(errno));
                continue;
            }

            (*num_entries)++;
        }
    }

    closedir(dir);
    return *num_entries > 0 ? 0 : -1;
}

static int is_directory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

static int process_target(const char *target, FileCardInfo **entries, int *num_entries, int *current_size) {
    if (is_directory(target)) {
        char previous_dir[MAX_PATH];
        if (getcwd(previous_dir, sizeof(previous_dir)) == NULL) {
            perror("getcwd() error");
            return -1;
        }

        if (chdir(target) != 0) {
            fprintf(stderr, "Error changing to directory '%s': %s\n", target, strerror(errno));
            return -1;
        }

        int result = process_directory(".", entries, num_entries, current_size);

        // Change back to the previous directory
        if (chdir(previous_dir) != 0) {
            fprintf(stderr, "Error changing back to previous directory: %s\n", strerror(errno));
            return -1;
        }

        return result;
    } else if (is_glob_pattern(target)) {
        return process_files_or_patterns(&target, 1, entries, num_entries, current_size);
    } else {
        // Assume it's a single file
        if (*num_entries >= *current_size) {
            if (allocate_entries(entries, current_size) != 0) {
                return -1;
            }
        }
        if (create_file_entry(&(*entries)[*num_entries], target) != 0) {
            fprintf(stderr, "Unable to get info for %s: %s\n", target, strerror(errno));
            return -1;
        }
        (*num_entries)++;
    }
    return 0;
}

// Add this new function to check if we're dealing with glob patterns or specific files
static int is_glob_or_specific_files(const char **targets, int target_count) {
    for (int i = 0; i < target_count; i++) {
        if (is_glob_pattern(targets[i]) || !is_directory(targets[i])) {
            return 1;
        }
    }
    return 0;
}


int main(int argc, char *argv[]) {
    CommandLineArgs args = parse_args(argc, argv);

    if (args.show_version) {
        print_version();
        free_args(&args);
        return EXIT_SUCCESS;
    }

    if (args.show_help) {
        print_help(argv[0]);
        free_args(&args);
        return EXIT_SUCCESS;
    }

    if (args.invalid_opt) {
        fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], args.invalid_opt);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        free_args(&args);
        return EXIT_FAILURE;
    }

    char current_dir[MAX_PATH];
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd() error");
        free_args(&args);
        return EXIT_FAILURE;
    }

    if (args.show_longlisting) {
        if (args.target_count > 0 && is_glob_or_specific_files(args.targets, args.target_count)) {
            print_longlisting_files(args.targets, args.target_count);
        } else {
            print_longlisting(args.target_count > 0 ? args.targets[0] : ".");
        }
        free_args(&args);
        return EXIT_SUCCESS;
    }

    if (args.show_dir_analytics) {
        print_dir_analytics(args.target_count > 0 ? args.targets[0] : ".");
        free_args(&args);
        return EXIT_SUCCESS;
    }

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;

    FileCardInfo *entries = NULL;
    int num_entries = 0;
    int current_size = 0;

    char display_path[MAX_PATH];
    strcpy(display_path, current_dir);

    int show_path = 1;

     if (args.target_count > 0) {
        show_path = 0;
        for (int i = 0; i < args.target_count; i++) {
            char *real_path = realpath(args.targets[i], NULL);
            if (real_path != NULL) {
                strcpy(display_path, real_path);
                free(real_path);
            } else {
                strcpy(display_path, args.targets[i]);
            }

            if (process_target(args.targets[i], &entries, &num_entries, &current_size) != 0) {
                fprintf(stderr, "Error processing '%s'\n", args.targets[i]);
                goto cleanup;
            }
        }
    } else {
        if (is_dev_directory(current_dir)) {
            strcpy(display_path, "/dev");
            if (handle_dev_directory(&entries, &num_entries, &current_size) != 0) {
                fprintf(stderr, "Error processing /dev directory\n");
                goto cleanup;
            }
        } else {
            if (process_directory(".", &entries, &num_entries, &current_size) != 0) {
                fprintf(stderr, "Error processing current directory\n");
                goto cleanup;
            }
        }
    }

       if (num_entries == 0) {
        char current_dir[PATH_MAX];
        if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
            if (strcmp(current_dir, display_path) == 0) {
                printf("\033[1m%s\033[0m\n", current_dir);
            }
            fprintf(stderr, "🚫 No files found\n");
        } else {
            fprintf(stderr, "🚫 No files found\n");
        }
    } else {
        qsort(entries, num_entries, sizeof(FileCardInfo), compare_file_entries);

        if (is_git_repository()) {
            GitFileList git_status = get_git_status();
            for (int i = 0; i < num_entries; i++) {
                for (int j = 0; j < git_status.count; j++) {
                    if (strcmp(entries[i].name, git_status.files[j].filename) == 0) {
                        entries[i].git_status[0] = git_status.files[j].status;
                        entries[i].git_status[1] = '\0';
                        break;
                    }
                }
            }
        }

        display_entries(entries, num_entries, term_width, display_path, show_path);
    }

cleanup:
    for (int i = 0; i < num_entries; i++) {
        free_file_entry(&entries[i]);
    }
    free(entries);
    free_args(&args);

    return num_entries > 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
