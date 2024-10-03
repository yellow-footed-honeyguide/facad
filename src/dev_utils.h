#ifndef DEV_UTILS_H
#define DEV_UTILS_H

#include "file_entry.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Checks if the specified path is the /dev directory.
     *
     * @param path Path to check.
     * @return 1 if the path is "/dev", otherwise 0.
     */
    int is_dev_directory(const char *path);

    /**
     * Handles the contents of the /dev directory and fills an array of FileCardInfo records.
     *
     * @param entries Double pointer to an array of FileCardInfo structures that will be populated.
     * @param num_entries Pointer to the current number of entries in the array.
     * @param max_entries Maximum number of entries that can be added.
     */
    void handle_dev_directory(FileCardInfo **entries, int *num_entries, int max_entries);

#ifdef __cplusplus
}
#endif

#endif // DEV_UTILS_H
