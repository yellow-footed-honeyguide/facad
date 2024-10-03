#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "dev_utils.h"
#include "emoji_utils.h"

#define MAX_PATH 4096 // Maximum path length

/**
 * Checks if the given path is the /dev directory.
 *
 * @param path The path to check.
 * @return 1 if the path is "/dev", otherwise 0.
 */
int is_dev_directory(const char *path)
{
    return strcmp(path, "/dev") == 0;
}

/**
 * Handles the contents of the /dev directory, filling an array of FileCardInfo structures.
 *
 * @param entries Pointer to an array of FileCardInfo structures that will be filled.
 * @param num_entries Pointer to the number of entries in the array.
 * @param max_entries The maximum number of entries that can be added to the array.
 */
void handle_dev_directory(FileCardInfo **entries, int *num_entries, int max_entries)
{
    DIR *dir = opendir("/dev");
    if (!dir)
    {
        perror("Error opening /dev");
        return;
    }

    struct dirent *d_entry;
    char full_path[MAX_PATH];
    struct stat st;

    while ((d_entry = readdir(dir)) != NULL && *num_entries < max_entries)
    {
        if (strcmp(d_entry->d_name, ".") == 0 || strcmp(d_entry->d_name, "..") == 0)
        {
            continue; // Skip the . and .. entries
        }

        // Construct the full path to the file in /dev
        snprintf(full_path, sizeof(full_path), "/dev/%s", d_entry->d_name);

        if (lstat(full_path, &st) == 0)
        {
            // Allocate a new FileCardInfo structure and populate it
            FileCardInfo *entry = &((*entries)[*num_entries]);
            entry->name = strdup(d_entry->d_name);
            if (entry->name == NULL)
            {
                perror("strdup");
                closedir(dir);
                return;
            }

            entry->is_directory = S_ISDIR(st.st_mode);
            entry->is_hidden = (d_entry->d_name[0] == '.');

            if (S_ISLNK(st.st_mode))
            {
                entry->emoji = strdup(entry->is_directory ? "🔗📁" : "🔗");
            }
            else if (entry->is_directory)
            {
                entry->emoji = strdup("📁");
            }
            else
            {
                entry->emoji = get_dev_emoji(d_entry->d_name);
            }

            if (entry->emoji == NULL)
            {
                perror("strdup/get_dev_emoji");
                free(entry->name); // Free allocated memory for the name in case of error
                closedir(dir);
                return;
            }

            (*num_entries)++; // Increment the number of entries
        }
    }

    closedir(dir);
}
