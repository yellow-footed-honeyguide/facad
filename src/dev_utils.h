#ifndef DEV_UTILS_H
#define DEV_UTILS_H

#include "file_entry.h"

int is_dev_directory(const char *path);
void handle_dev_directory(FileEntry **entries, int *num_entries, int max_entries);

#endif // DEV_UTILS_H
