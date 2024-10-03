/**
 * @file dir_analytics.h
 * @brief Header file for directory analysis functionality.
 *
 * This file contains the declaration of functions for analyzing
 * and displaying detailed information about the contents of a directory.
 */

#ifndef DIR_ANALYTICS_H
#define DIR_ANALYTICS_H

/**
 * @brief Prints detailed analytics about a directory.
 *
 * This function analyzes the contents of the specified directory
 * and prints various statistics such as total size, number of files,
 * directories, file sizes, modification times, etc.
 *
 * The analytics include:
 * - Path of the directory
 * - Creation and modification times
 * - Owner and permissions
 * - Total size of all files
 * - Number of files and directories
 * - Directory tree depth
 * - Number of hidden items
 * - Largest file and size range
 * - Newest and oldest files
 * - Number of symlinks and empty files
 * - File to directory ratio
 * - Unique file extensions
 *
 * @param path The path of the directory to analyze.
 */
void print_dir_analytics(const char *path);

#endif // DIR_ANALYTICS_H

