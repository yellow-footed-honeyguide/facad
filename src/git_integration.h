/**
 * @file git_integration.h
 * @brief Header file for Git integration functionality.
 *
 * This file declares the interface for integrating Git status information
 * into the file entries displayed by the facad tool. It provides the
 * necessary function declarations and includes required dependencies.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef GIT_INTEGRATION_H
#define GIT_INTEGRATION_H

#include "file_card.h"  // Include for FileCardInfo structure
#include "git_utils.h"  // Include for Git-related utility functions

/**
 * @brief Integrates Git status information into file entries.
 *
 * This function retrieves the Git status for files in the current directory
 * and updates the corresponding FileCardInfo structures with this information.
 * It should be called after file entries are created but before they are displayed.
 *
 * @param entries Pointer to an array of FileCardInfo pointers. Each entry represents a file or directory.
 * @param num_entries Pointer to the number of entries in the array. This allows for potential modification of the entry count.
 * @param current_dir The current directory path. This is used to construct relative paths for Git status comparison.
 *
 * @note This function modifies the git_status field of each FileCardInfo structure in the entries array.
 * @note If the current directory is not a Git repository, this function will have no effect.
 */
void integrate_git_status(FileCardInfo** entries, int* num_entries, const char* current_dir);

#endif // GIT_INTEGRATION_H
