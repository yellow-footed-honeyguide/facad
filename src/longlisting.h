/**
 * @file longlisting.h
 * @brief Header file for detailed directory listing functionality.
 *
 * This file contains the declaration of functions for generating
 * and displaying a detailed listing of directory contents, including
 * file sizes, permissions, modification times, and other attributes.
 */
#ifndef LONGLISTING_H
#define LONGLISTING_H

/**
 * @brief Prints a detailed listing of the contents of a directory.
 *
 * This function reads the contents of the specified directory,
 * sorts them, and prints a detailed listing including:
 * - File size (human-readable format)
 * - Last modification time
 * - File permissions (using emoji representations)
 * - File type emoji
 * - File name
 * - For directories, the number of subdirectories
 *
 * The listing is sorted with directories first, then by file extension,
 * then by size, and finally by name.
 *
 * @param path The path of the directory to list.
 *
 * @note This function uses UTF-8 encoding for emoji output, so the
 *       terminal or console should support UTF-8 for proper display.
 *
 * @warning This function has a limit of MAX_ENTRIES (1000 by default)
 *          entries it can process. If a directory contains more entries,
 *          only the first MAX_ENTRIES will be displayed.
 */
void print_longlisting(const char *path);

#endif // LONGLISTING_H

