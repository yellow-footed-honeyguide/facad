/**
 * @file longlisting.h
 * @brief Header file for detailed directory listing functionality.
 *
 * This file contains declarations for functions that generate and display
 * detailed listings of directory contents, including file sizes, permissions,
 * modification times, and other attributes.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef LONGLISTING_H
#define LONGLISTING_H

/**
 * @brief Prints a detailed listing of the contents of a directory.
 *
 * This function reads the contents of the specified directory,
 * sorts them, and prints a detailed listing including:
 * - File size (human-readable format)
 * - Last modification time (relative to current time)
 * - File owner
 * - File permissions (using emoji representations)
 * - File type emoji
 * - File name
 * - For directories, the number of subdirectories
 *
 * The listing is sorted with directories first, then by size
 * (larger files/directories first), and finally alphabetically by name.
 *
 * @param path The path of the directory to list.
 *
 * @note This function uses UTF-8 encoding for emoji output, so the
 *       terminal or console should support UTF-8 for proper display.
 *
 * @warning This function has a limit of MAX_ENTRIES (1000 by default)
 *          entries it can process. If a directory contains more entries,
 *          only the first MAX_ENTRIES will be displayed.
 *
 * @see format_size() for size formatting
 * @see format_time_ago() for time formatting
 * @see get_user_rights() for permission formatting
 * @see get_emoji() for file type emoji selection
 */
void print_longlisting(const char *path);

// Additional function declarations can be added here if needed

#endif // LONGLISTING_H
