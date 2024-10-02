// (f)function, (t)type, (m)macros, (s)structure
#include <dirent.h>    // DIR (t), struct dirent (s), opendir (f), readdir (f), closedir (f)
#include <errno.h>     // errno (m)
#include <stdio.h>     // printf (f), fprintf (f), perror (f)
#include <stdlib.h>    // malloc (f), free (f)
#include <string.h>    // strcmp (f), strerror (f)
#include <sys/ioctl.h> // ioctl (f), struct winsize (s)
#include <unistd.h>    // getcwd (f), STDOUT_FILENO (m)

#include "args_parser.h"   // CommandLineArgs (s), parse_args (f)
#include "dev_utils.h"     // is_dev_directory (f), handle_dev_directory (f)
#include "display_utils.h" // display_entries (f)
#include "file_entry.h"    // FileEntry (s), create_file_entry (f), free_file_entry (f), compare_file_entries (f)
#include "longlisting.h"
#include "dir_analytics.h"

#define MAX_PATH 4096    // Maximum path length (m)
#define MAX_ENTRIES 1024 // Maximum number of directory entries (m)

int main(int argc, char *argv[])
{
    CommandLineArgs args = parse_args(argc, argv); // Parse command line arguments
    if (args.show_version)
    {                    // If --version flag is set
        print_version(); // Print version information
        return 0;        // Exit program
    }
    if (args.show_help)
    {                        // If --help flag is set
        print_help(argv[0]); // Print help information
        return 0;            // Exit program
    }

    if (args.show_longlisting) {
        print_longlisting(".");
        return 0;
    }

    if (args.show_dir_analytics) {
        print_dir_analytics(".");
        return 0;
    }

    char current_dir[MAX_PATH]; // Buffer to store current directory path
    struct winsize w;           // Structure to hold terminal window size
    int term_width;             // Terminal width

    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {                             // Get current working directory
        perror("getcwd() error"); // Print error message if getcwd() fails
        return 1;                 // Exit with error code
    }

    printf("\033[1m%s\033[0m\n", current_dir); // Print current directory in bold

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Get terminal window size
    term_width = w.ws_col;                // Store terminal width

    // Allocate memory for file entries
    FileEntry *entries = malloc(MAX_ENTRIES * sizeof(FileEntry));
    if (entries == NULL)
    {                             // Check if memory allocation failed
        perror("malloc() error"); // Print error message
        return 1;                 // Exit with error code
    }

    int num_entries = 0; // Counter for number of entries

    if (is_dev_directory(current_dir))
    { // Check if current directory is /dev
        // Handle /dev directory specially
        handle_dev_directory(&entries, &num_entries, MAX_ENTRIES);
    }
    else
    {
        DIR *dir = opendir("."); // Open current directory
        if (dir == NULL)
        {                              // Check if directory opening failed
            perror("opendir() error"); // Print error message
            free(entries);             // Free allocated memory
            return 1;                  // Exit with error code
        }

        struct dirent *entry; // Pointer to directory entry
        while ((entry = readdir(dir)) != NULL && num_entries < MAX_ENTRIES)
        { // Read directory entries
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            { // Skip . and .. entries
                continue;
            }

            if (create_file_entry(&entries[num_entries], entry->d_name) != 0)
            { // Create file entry
                // Print warning if creation fails
                fprintf(stderr, "Warning: Unable to get info for %s: %s\n", entry->d_name, strerror(errno));
                continue; // Skip to next entry
            }
            num_entries++; // Increment entry count
        }
        closedir(dir); // Close directory
    }

    // Sort entries
    qsort(entries, num_entries, sizeof(FileEntry), compare_file_entries);

    // Display sorted entries
    display_entries(entries, num_entries, term_width);

    // Free each entry
    for (int i = 0; i < num_entries; i++)
    {
        free_file_entry(&entries[i]);
    }
    free(entries); // Free entries array

    return 0; // Exit successfully
}
