/**
 * @file display_utils.h
 * @brief Header file for display utilities in facad
 *
 * This file contains declarations for functions that handle
 * the display formatting and output of file entries in a
 * grid-like format, taking into account terminal width and
 * Unicode character handling.
 *
 * @author Sergey Veneckiy
 * @date 2024
 */

#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include "file_card.h"

/**
 * @brief Displays a list of FileCardInfo objects in a grid format
 *
 * This function takes an array of FileCardInfo structures and displays
 * them in a formatted grid layout. It considers the terminal width to
 * optimize the display and handles Unicode characters properly.
 *
 * The display includes:
 * - File/directory emojis
 * - File/directory names
 * - Proper alignment and spacing
 *
 * @param entries Pointer to an array of FileCardInfo structures to be displayed
 * @param num_entries The number of entries in the array
 * @param term_width The width of the terminal in characters
 *
 * @note This function assumes that the entries have already been sorted
 *       according to the desired display order.
 *
 * @warning The function does not perform bounds checking on the entries array.
 *          Ensure that num_entries accurately reflects the array size.
 *
 * @see FileCardInfo
 * @see get_display_width() for Unicode character width calculation
 * @see print_padded() for formatted output of each entry
 */
void display_entries(FileCardInfo *entries, int num_entries, int term_width);

// Additional function declarations can be added here if needed

#endif // DISPLAY_UTILS_H
