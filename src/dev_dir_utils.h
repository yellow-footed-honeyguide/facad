/**
 * @file dev_dir_utils.h
 * @brief Utility functions for handling the /dev directory
 *
 * This header file declares functions for identifying and processing
 * entries in the /dev directory, which contains device files on Unix-like systems.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef DEV_UTILS_H
#define DEV_UTILS_H

#include "file_card.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Checks if the specified path is the /dev directory
     *
     * This function determines whether the given path represents
     * the /dev directory.
     *
     * @param path Path to check
     * @return 1 if the path is "/dev", 0 otherwise
     */
    int is_dev_directory(const char *path);

    /**
     * @brief Processes the contents of the /dev directory
     *
     * This function reads and processes the entries in the /dev directory,
     * creating FileCardInfo structures for each entry and storing them
     * in the provided array. It handles memory allocation and reallocation
     * as needed.
     *
     * @param entries Double pointer to an array of FileCardInfo structures that will be populated
     * @param num_entries Pointer to the current number of entries in the array
     * @param current_size Pointer to the current size of the array
     * @return 0 on success, -1 on failure
     */
    int handle_dev_directory(FileCardInfo **entries, int *num_entries, int *current_size);

#ifdef __cplusplus
}
#endif

#endif // DEV_UTILS_H
