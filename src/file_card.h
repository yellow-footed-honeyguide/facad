/**
 * @file file_card.h
 * @brief Header file for file entry handling functions and structures.
 *
 * This file contains the definition of the FileCardInfo structure and
 * declarations for functions that manipulate file entries. It provides
 * the interface for creating, freeing, and comparing file entries in
 * the facad directory listing tool.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef FILE_ENTRY_H
#define FILE_ENTRY_H

/**
 * @struct FileCardInfo
 * @brief Structure representing a file entry with its properties.
 *
 * This structure holds all the necessary information about a file or directory
 * entry, including its name, associated emoji, and various flags indicating
 * its properties.
 */
typedef struct
{
    char *name;         /**< Name of the file or directory */
    char *emoji;        /**< Emoji representation of the file type */
    int is_directory;   /**< Flag indicating whether the entry is a directory (1) or not (0) */
    int is_hidden;      /**< Flag indicating whether the entry is hidden (1) or not (0) */
    char git_status[2]; /**< Git status of the file (if applicable) */
} FileCardInfo;

/**
 * @brief Creates a file entry based on the given path.
 *
 * This function initializes a FileCardInfo structure with information about the file,
 * including its name, emoji representation, and flags for directory and hidden status.
 *
 * @param entry Pointer to a FileCardInfo structure to be filled.
 * @param path Path of the file to analyze.
 * @return 0 on success, non-zero on error.
 */
int create_file_entry(FileCardInfo *entry, const char *path);

/**
 * @brief Frees the memory allocated for a file entry.
 *
 * This function releases the memory allocated for the name and emoji fields
 * of a FileCardInfo structure.
 *
 * @param entry Pointer to the FileCardInfo structure to free.
 */
void free_file_entry(FileCardInfo *entry);

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
 * @return A negative value if a should come before b,
 *         a positive value if b should come before a,
 *         or 0 if they are equivalent.
 */
int compare_file_entries(const void *a, const void *b);

#endif // FILE_ENTRY_H
