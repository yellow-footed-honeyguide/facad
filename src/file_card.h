/**
 * @file file_card.h
 * @brief Definitions and functions for handling file entries
 *
 * This header file defines the FileCardInfo structure and declares
 * functions for creating, freeing, and comparing file entries. It
 * provides the core data structure and operations used throughout
 * the facad directory listing tool.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef FILE_ENTRY_H
#define FILE_ENTRY_H

/**
 * @struct FileCardInfo
 * @brief Represents information about a file or directory entry
 *
 * This structure contains various attributes of a file or directory,
 * including its name, emoji representation, flags for directory and
 * hidden status, Git status, and a flag for modified contents.
 */
typedef struct
{
    char *name;              /**< Name of the file or directory */
    char *emoji;             /**< Emoji representation of the file type */
    int is_directory;        /**< Flag indicating if the entry is a directory (1) or file (0) */
    int is_hidden;           /**< Flag indicating if the entry is hidden (1) or visible (0) */
    char git_status[2];      /**< Git status of the file (2 characters for status code) */
    int contains_modified;   /**< Flag indicating if a directory contains modified files (1) or not (0) */
} FileCardInfo;

/**
 * @brief Creates a new file entry based on the given path
 *
 * This function initializes a FileCardInfo structure with information
 * about the file or directory at the specified path.
 *
 * @param entry Pointer to a FileCardInfo structure to be filled
 * @param path Path of the file or directory to analyze
 * @return 0 on success, -1 on error
 */
int create_file_entry(FileCardInfo *entry, const char *path);

/**
 * @brief Frees memory allocated for a file entry
 *
 * This function releases the memory allocated for the name and emoji
 * fields of a FileCardInfo structure.
 *
 * @param entry Pointer to the FileCardInfo structure to free
 */
void free_file_entry(FileCardInfo *entry);

/**
 * @brief Compares two file entries for sorting
 *
 * This function implements the sorting logic for file entries:
 * 1. Directories before files
 * 2. Within directories and files, dotfiles (hidden) before non-dotfiles
 * 3. Files sorted by extension
 * 4. Alphabetical sorting within the same type and extension
 *
 * @param a Pointer to the first FileCardInfo structure
 * @param b Pointer to the second FileCardInfo structure
 * @return An integer less than, equal to, or greater than zero if a is found,
 *         respectively, to be less than, to match, or be greater than b
 */
int compare_file_entries(const void *a, const void *b);

#endif // FILE_ENTRY_H
