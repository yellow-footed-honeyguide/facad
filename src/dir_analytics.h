/**
 * @file dir_analytics.h
 * @brief Header file for directory analysis functionality
 *
 * This file contains the declaration of functions for generating
 * and displaying detailed analytics about directory contents.
 * It provides an interface for examining file sizes, counts,
 * timestamps, and other attributes within a directory structure.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef DIR_ANALYTICS_H
#define DIR_ANALYTICS_H

/**
 * @brief Prints detailed analytics about a directory
 *
 * This function analyzes the contents of the specified directory
 * and prints various statistics including:
 * - Total size of all files
 * - Number of files and directories
 * - Depth of the directory tree
 * - File size ranges (smallest and largest)
 * - Newest and oldest files
 * - File type distributions
 * - Permissions and ownership information
 *
 * The analytics provide a comprehensive overview of the directory's
 * contents, useful for storage management and content analysis.
 *
 * @param path The path of the directory to analyze
 *
 * @note This function performs a recursive analysis and may take
 *       considerable time for large directory structures.
 *
 * @warning For very large directories, this function may consume
 *          significant memory and processing resources.
 */
void print_dir_analytics(const char *path);

#endif // DIR_ANALYTICS_H
