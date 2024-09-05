// src/file_entry.h

#ifndef FILE_ENTRY_H
#define FILE_ENTRY_H

/**
 * Structure representing a file entry with its properties.
 */
typedef struct
{
    char *name;       // Name of the file
    char *emoji;      // Emoji representation of the file type
    int is_directory; // Flag indicating if the entry is a directory
    int is_hidden;    // Flag indicating if the entry is hidden
} FileEntry;

/**
 * Creates a file entry based on the given path.
 *
 * @param entry Pointer to a FileEntry structure to be filled.
 * @param path Path of the file to analyze.
 * @return 0 on success, non-zero on error.
 */
int create_file_entry(FileEntry *entry, const char *path);

/**
 * Frees the memory allocated for a file entry.
 *
 * @param entry Pointer to the FileEntry structure to free.
 */
void free_file_entry(FileEntry *entry);

/**
 * Compares two file entries for sorting purposes.
 *
 * @param a Pointer to the first FileEntry structure.
 * @param b Pointer to the second FileEntry structure.
 * @return A negative value if the first entry should come before the second,
 *         a positive value if the first entry should come after the second,
 *         or 0 if they are equivalent.
 */
int compare_file_entries(const void *a, const void *b);

#endif // FILE_ENTRY_H
