/**
 * @file file_card.c
 * @brief Implementation of file card handling functions.
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
#include <ctype.h>

#include "emoji_utils.h"
#include "file_card.h"

/**
 * @brief Creates a new file card based on the given path.
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
 * @brief Compare two strings case-insensitively.
 *
 * This function provides a case-insensitive string comparison,
 * similar to strcasecmp, but implemented for portability.
 *
 * @param a The first string to compare.
 * @param b The second string to compare.
 * @return An integer less than, equal to, or greater than zero if a is found,
 *         respectively, to be less than, to match, or be greater than b.
 */
static int strcasecmp_wrapper(const char *a, const char *b) {
    // Loop continues as long as both strings have characters
    while (*a && *b) {
        // Convert current characters to lowercase for case-insensitive comparison
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        
        // If characters differ, return their difference (determines sorting order)
        if (ca != cb) return ca - cb; 
        
        // Move to the next character in both strings
        a++;
        b++;
    }   
    
    // If we've reached here, one string might be longer than the other
    // Compare the last characters (one might be '\0' if strings are different lengths)
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

/**
 * @brief Compare two file entries for sorting.
 *
 * This function implements the sorting logic for file entries:
 * 1. Directories before files
 * 2. Within directories and files, dotfiles (hidden) before non-dotfiles
 * 3. Files sorted by extension
 * 4. Alphabetical sorting within the same type and extension
 *
 * @param a Pointer to the first FileCardInfo structure.
 * @param b Pointer to the second FileCardInfo structure.
 * @return An integer less than, equal to, or greater than zero if a is found,
 *         respectively, to be less than, to match, or be greater than b.
 */
int compare_file_entries(const void *a, const void *b)
{
    const FileCardInfo *entry_a = (const FileCardInfo *)a;
    const FileCardInfo *entry_b = (const FileCardInfo *)b;

    // Sort directories before files
    if (entry_a->is_directory != entry_b->is_directory) {
        return entry_b->is_directory - entry_a->is_directory;
    }

    // For directories, sort dotfiles first
    if (entry_a->is_directory && entry_b->is_directory) {
        // Check if one is a dotfile and the other isn't
        if ((entry_a->name[0] == '.') != (entry_b->name[0] == '.')) {
            return (entry_b->name[0] == '.') - (entry_a->name[0] == '.');
        }
        // Both are either dotfiles or non-dotfiles, sort alphabetically
        return strcasecmp_wrapper(entry_a->name, entry_b->name);
    }

    // For files, sort dotfiles first
    if ((entry_a->name[0] == '.') != (entry_b->name[0] == '.')) {
        return (entry_b->name[0] == '.') - (entry_a->name[0] == '.');
    }

    // Sort files by extension
    const char *ext_a = get_extension(entry_a->name);
    const char *ext_b = get_extension(entry_b->name);
    int ext_cmp = strcasecmp_wrapper(ext_a, ext_b);

    // If extensions differ, return their comparison result
    if (ext_cmp != 0) {
        return ext_cmp;
    }

    // If extensions are the same, sort alphabetically by name
    return strcasecmp_wrapper(entry_a->name, entry_b->name);
}
