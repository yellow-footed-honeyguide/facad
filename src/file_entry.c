#include <stdlib.h> 
#include <string.h>
#include <sys/stat.h>

#include "emoji_utils.h"
#include "file_entry.h"

// Create a new file card info
int create_file_entry(FileCardInfo *entry, const char *path)
{
    entry->name = strdup(path);     // Duplicate the path string
    entry->emoji = get_emoji(path); // Get the appropriate emoji for the file

    struct stat path_stat; // Structure to hold file stats
    if (lstat(path, &path_stat) != 0)
    {              // Get file stats, using lstat to handle symlinks
        return -1; // Return error if unable to get file info
    }

    entry->is_directory = S_ISDIR(path_stat.st_mode); // Check if it's a directory
    entry->is_hidden = (path[0] == '.');              // Check if it's a hidden file/directory

    return 0; // Successful entry creation
}

// Free memory allocated for a file entry
void free_file_entry(FileCardInfo *entry)
{
    free(entry->name);  // Free the name string
    free(entry->emoji); // Free the emoji string
}

// Get the file extension from a filename
char *get_extension(const char *name)
{
    char *dot = strrchr(name, '.'); // Find the last occurrence of '.'
    if (!dot || dot == name)
    {              // If no '.' found or it's the first character
        return ""; // Return empty string (no extension)
    }
    return dot + 1; // Return pointer to character after the last '.'
}

// Compare two file entries for sorting
int compare_file_entries(const void *a, const void *b)
{
    const FileCardInfo *entry_a = (const FileCardInfo *)a;
    const FileCardInfo *entry_b = (const FileCardInfo *)b;

    // Sort directories first
    if (entry_a->is_directory && !entry_b->is_directory)
    {
        return -1; // a comes before b
    }
    if (!entry_a->is_directory && entry_b->is_directory)
    {
        return 1; // b comes before a
    }

    // Then sort hidden files and directories
    if (entry_a->is_hidden && !entry_b->is_hidden)
    {
        return -1; // Hidden entries come first
    }
    if (!entry_a->is_hidden && entry_b->is_hidden)
    {
        return 1; // Non-hidden entries come after
    }

    // Finally, sort by name (case-insensitive)
    return strcasecmp(entry_a->name, entry_b->name);
}
