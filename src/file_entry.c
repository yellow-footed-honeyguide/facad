/**
 * @file file_entry.c
 * @brief Implementation of file entry handling functions.
 *
 * This file contains functions for creating, freeing, and comparing file entries,
 * as well as utility functions for working with file names and extensions.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#include <stdlib.h> 
#include <string.h>
#include <sys/stat.h>

#include "emoji_utils.h"
#include "file_entry.h"

/**
 * @brief Creates a new file entry based on the given path.
 *
 * This function initializes a FileCardInfo structure with information about the file,
 * including its name, emoji representation, and flags for directory and hidden status.
 *
 * @param entry Pointer to a FileCardInfo structure to be filled.
 * @param path Path of the file to analyze.
 * @return 0 on success, -1 on error.
 */
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

/**
 * @brief Frees memory allocated for a file entry.
 *
 * This function releases the memory allocated for the name and emoji fields
 * of a FileCardInfo structure.
 *
 * @param entry Pointer to the FileCardInfo structure to free.
 */
void free_file_entry(FileCardInfo *entry)
{
    free(entry->name);  // Free the name string
    free(entry->emoji); // Free the emoji string
}

/**
 * @brief Extracts the file extension from a filename.
 *
 * This function finds the last occurrence of a dot in the filename and
 * returns the substring following it as the file extension.
 *
 * @param name The filename to analyze.
 * @return A pointer to the file extension, or an empty string if no extension is found.
 */
char *get_extension(const char *name)
{
    char *dot = strrchr(name, '.'); // Find the last occurrence of '.'
    if (!dot || dot == name)
    {              // If no '.' found or it's the first character
        return ""; // Return empty string (no extension)
    }
    return dot + 1; // Return pointer to character after the last '.'
}

/**
 * @brief Compares two file entries for sorting purposes.
 *
 * This function implements the sorting logic for file entries. It prioritizes:
 * 1. Directories before files
 * 2. Hidden entries before non-hidden entries
 * 3. Alphabetical order (case-insensitive) for entries of the same type
 *
 * @param a Pointer to the first FileCardInfo structure.
 * @param b Pointer to the second FileCardInfo structure.
 * @return Negative value if a should come before b, positive if b should come before a, 0 if equivalent.
 */
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
