#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include "file_entry.h"

/**
 * Displays a list of FileCardInfo objects in a grid format.
 *
 * @param entries Pointer to an array of FileCardInfo structures.
 * @param num_entries The number of entries in the array.
 * @param term_width The width of the terminal in characters.
 */
void display_entries(FileCardInfo *entries, int num_entries, int term_width);

#endif // DISPLAY_UTILS_H
