// src/display_utils.h

#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include "file_entry.h"

/**
 * Displays a list of FileEntry objects in a grid format.
 *
 * @param entries Pointer to an array of FileEntry structures.
 * @param num_entries The number of entries in the array.
 * @param term_width The width of the terminal in characters.
 */
void display_entries(FileEntry *entries, int num_entries, int term_width);

#endif // DISPLAY_UTILS_H
